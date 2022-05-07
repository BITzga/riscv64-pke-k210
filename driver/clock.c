/**
 * @Author : zhangguoan
 * @Date :  2022/5/2.
 */

#include "clock.h"
#include "../spike_interface/spike_utils.h"
#include "../kernel/riscv.h"
#include "../k210_lib/sbi.h"

volatile uint64 g_ticks;

static inline uint64 get_cycles(void) {
#if __riscv_xlen == 64
    uint64 n;
    __asm__ __volatile__("rdtime %0" : "=r"(n));
    return n;
#else
    uint32_t lo, hi, tmp;
    __asm__ __volatile__(
    "1:\n"
    "rdtimeh %0\n"
    "rdtime %1\n"
    "rdtimeh %2\n"
    "bne %0, %2, 1b"
    : "=&r"(hi), "=&r"(lo), "=&r"(tmp));
    return ((uint64) hi << 32) | lo;
#endif
}


// Hardcode timebase
static uint64 timebase = 10000000;


/* *
 * clock_init - initialize 8253 clock to interrupt 100 times per second,
 * and then enable IRQ_TIMER.
 * */
void clock_init(void) {
    // enable timer interrupt in sie
    set_csr(sie, MIP_STIP);
    // divided by 500 when using Spike(2MHz)
    // divided by 100 when using QEMU(10MHz)
    // timebase = sbi_timebase() / 500;
    clock_set_next_event();

    // initialize time counter 'ticks' to zero
    g_ticks = 0;

    sprint("++ setup timer interrupts\n");
}


void clock_set_next_event(void) { sbi_set_timer(get_cycles() + timebase); }
