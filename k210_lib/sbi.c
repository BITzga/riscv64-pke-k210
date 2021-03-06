/**
 * @Author : zhangguoan
 * @Date :  2022/4/1.
 */

#include "../util/types.h"
#include "sbi.h"

uint64 SBI_SET_TIMER = 0;
uint64 SBI_CONSOLE_PUTCHAR = 1;
uint64 SBI_CONSOLE_GETCHAR = 2;
uint64 SBI_CLEAR_IPI = 3;
uint64 SBI_SEND_IPI = 4;
uint64 SBI_REMOTE_FENCE_I = 5;
uint64 SBI_REMOTE_SFENCE_VMA = 6;
uint64 SBI_REMOTE_SFENCE_VMA_ASID = 7;
uint64 SBI_SHUTDOWN = 8;

uint64 sbi_call(uint64 sbi_type, uint64 arg0, uint64 arg1, uint64 arg2) {
    uint64 ret_val;
    __asm__ volatile (
    "mv x17, %[sbi_type]\n"
    "mv x10, %[arg0]\n"
    "mv x11, %[arg1]\n"
    "mv x12, %[arg2]\n"
    "ecall\n"
    "mv %[ret_val], x10"
    : [ret_val] "=r"(ret_val)
    : [sbi_type] "r"(sbi_type), [arg0] "r"(arg0), [arg1] "r"(arg1), [arg2] "r"(arg2)
    : "memory"
    );
    return ret_val;
}

void sbi_console_putchar(unsigned char ch) {
    sbi_call(SBI_CONSOLE_PUTCHAR, ch, 0, 0);
}

void sbi_shutdown(void) {
    sbi_call(SBI_SHUTDOWN, 0, 0, 0);
}

void sbi_set_timer(unsigned long long stime_value) {
    sbi_call(SBI_SET_TIMER, stime_value, 0, 0);
}