/**
 * @Author : zhangguoan
 * @Date :  2022/4/2.
 */

#include "../driver/console.h"
#include "stdio.h"

/* *
 * cputch - writes a single character @c to stdout, and it will
 * increace the value of counter pointed by @cnt.
 * */
static void cputch(int c, int *cnt) {
    cons_putc(c);
    (*cnt)++;
}


/* cputchar - writes a single character to stdout */
void cputchar(int c) {
    cons_putc(c);
}


/* *
 * cputs- writes the string pointed by @str to stdout and
 * appends a newline character.
 * */
int cputs(const char *str) {
    int cnt = 0;
    char c;
    while ((c = *str++) != '\0') {
        cputch(c, &cnt);
    }
    cputch('\n', &cnt);
    return cnt;
}