/**
 * @Author : zhangguoan
 * @Date :  2022/5/2.
 */

#ifndef RISCV64_PKE_K210_CLOCK_H
#define RISCV64_PKE_K210_CLOCK_H

#include "../util/types.h"

// global variable that store the recorded "ticks"

extern volatile uint64 g_ticks;

void clock_init(void);

void clock_set_next_event(void);

#endif //RISCV64_PKE_K210_CLOCK_H
