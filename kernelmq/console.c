#include "console.h"

#include <kernaux/arch/i386.h>
#include <kernaux/stdlib.h>

void console_print(const char *const s)
{
    console_write(s, kernaux_strlen(s));
}

void console_putc(const char c) {
    kernaux_arch_i386_outportb(0x3F8, c);
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
