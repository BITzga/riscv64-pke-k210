/**
 * @Author : zhangguoan
 * @Date :  2022/4/1.
 */


#ifndef RISCV_PKE_SBI_H
#define RISCV_PKE_SBI_H


void sbi_set_timer(unsigned long long stime_value);

void sbi_shutdown(void);

void sbi_console_putchar(unsigned char ch);


#endif //RISCV_PKE_SBI_H
