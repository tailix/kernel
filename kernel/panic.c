#include "panic.h"

#include <kernaux/asm/i386.h>
#include <kernaux/console.h>

void panic(const char *const s)
{
    kernaux_console_printf("[FAIL] panic: %s\n", s);
    kernaux_asm_i386_hang();
}

void halt()
{
    panic("Kernel main function returned.");
}

void kernaux_assert_fn(
    const char *const file,
    const int line,
    const char *const str
) {
    kernaux_console_printf("[FAIL] assertion failed: %s:%u: \"%s\"\n",
                           file, line, str);
    kernaux_asm_i386_hang();
}
