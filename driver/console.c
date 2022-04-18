/**
 * @Author : zhangguoan
 * @Date :  2022/4/2.
 */
#include "../k210_lib/sbi.h"
#include "console.h"


void cons_putc(int c) {
    sbi_console_putchar((unsigned char) c);
}

