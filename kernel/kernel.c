/*
 * Supervisor-mode startup codes
 */

#include "riscv.h"
#include "string.h"
#include "elf.h"
#include "process.h"

#include "../spike_interface/spike_utils.h"

process user_app;

//
// load the elf, and construct a "process" (with only a trapframe).
// load_bincode_from_host_elf is defined in elf.c
//
void load_user_program(process *proc) {
  proc->trapframe = (trapframe *)USER_TRAP_FRAME;
  memset(proc->trapframe, 0, sizeof(trapframe));
  proc->kstack = USER_KSTACK;
  proc->trapframe->regs.sp = USER_STACK;

  load_bincode_from_host_elf(proc);
}

void load_user_program_on_k210(process *proc) {
    proc->trapframe = (trapframe *)USER_TRAP_FRAME;
    memset(proc->trapframe, 0, sizeof(trapframe));
    proc->kstack = USER_KSTACK;
    proc->trapframe->regs.sp = USER_STACK;
    proc->trapframe->epc = USER_PROGRAM_ENTRY;

    sprint("Application program entry point (virtual address): 0x%lx\n", proc->trapframe->epc);
}

//
// s_start: S-mode entry point of PKE OS kernel.
//
int s_start(void) {
  sprint("Enter supervisor mode...\n");
  // Note: we use direct (i.e., Bare mode) for memory mapping in lab1.
  // which means: Virtual Address = Physical Address
  write_csr(satp, 0);

  // the application code (elf) is first loaded into memory, and then put into execution
  load_user_program_on_k210(&user_app);

  sprint("Switch to user mode...\n");
  switch_to(&user_app);

  return 0;
}
