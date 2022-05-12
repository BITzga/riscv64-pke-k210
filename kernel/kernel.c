/*
 * Supervisor-mode startup codes
 */

#include "riscv.h"
#include "string.h"
#include "elf.h"
#include "process.h"
#include "pmm.h"
#include "vmm.h"
#include "sched.h"
#include "memlayout.h"
#include "../spike_interface/spike_utils.h"
#include "../driver/clock.h"

//
// trap_sec_start points to the beginning of S-mode trap segment (i.e., the entry point of
// S-mode trap vector).
//
extern char trap_sec_start[];

//
// turn on paging.
//
void enable_paging() {
    // write the pointer to kernel page (table) directory into the CSR of "satp".
    write_csr(satp, MAKE_SATP(g_kernel_pagetable));

    // refresh tlb to invalidate its content.
    flush_tlb();
}

//
// load the elf, and construct a "process" (with only a trapframe).
// load_bincode_from_host_elf is defined in elf.c
//
process *load_user_program() {
    process *proc;

    proc = alloc_process();

    sprint("User application is loading.\n");
    load_bincode_from_host_elf(proc);

    return proc;
}

extern char _etext[];

process *load_user_program_on_k210() {
    process *proc;

    proc = alloc_process();

    sprint("User application is loading.\n");
    user_vm_map((pagetable_t) proc->pagetable, USER_PROGRAM_ENTRY, (uint64) _etext - USER_PROGRAM_ENTRY,
                USER_PROGRAM_ENTRY,
                prot_to_type(PROT_WRITE | PROT_EXEC | PROT_READ, 1));

    // record the vm region in proc->mapped_info
    int j;
    for (j = 0; j < PGSIZE / sizeof(mapped_region); j++) {
        if (proc->mapped_info[j].va == 0x0) break;
    }

    proc->mapped_info[j].va = USER_PROGRAM_ENTRY;
    proc->mapped_info[j].npages = 1;
    proc->mapped_info[j].seg_type = CODE_SEGMENT;
    proc->total_mapped_region++;
    proc->trapframe->epc = USER_PROGRAM_ENTRY;

    sprint("CODE_SEGMENT added at mapped info offset:%d\n", j);
    sprint("Application program entry point (virtual address): 0x%lx\n", proc->trapframe->epc);

    return proc;
}

//
// s_start: S-mode entry point of PKE OS kernel.
//
int s_start(void) {
    sprint("Enter supervisor mode...\n");
    // in the beginning, we use Bare mode (direct) memory mapping as in lab1,
    // but now switch to paging mode in lab2.
    write_csr(satp, 0);

    // init clock
    clock_init();

    // init phisical memory manager
    pmm_init();

    // build the kernel page table
    kern_vm_init();

    // now, switch to paging mode by turning on paging (SV39)
    enable_paging();
    sprint("kernel page table is on \n");

    init_proc_pool();

    // the application code (elf) is first loaded into memory, and then put into execution
    sprint("Switch to user mode...\n");
    insert_to_ready_queue(load_user_program_on_k210());
    schedule();

    return 0;
}
