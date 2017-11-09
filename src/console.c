#include "console.h"

#include "stdlib.h"
#include "asm.h"

void console_print(const char *const s)
{
    console_write(s, kstrlen(s));
}

void console_putc(const char c) {
    outportb(0x3F8, c);
}

void console_puts(const char *const s)
{
    console_print(s);
    console_putc('\n');
}

void console_write(const char *const data, const unsigned int size) {
    for (unsigned int i = 0; i < size; i++) {
        console_putc(data[i]);
    }
}
