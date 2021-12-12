#include "panic.h"

#include <kernaux/arch/i386.h>
#include <kernaux/console.h>

void panic(const char *const s)
{
    kernaux_console_printf("[FAIL] panic: %s\n", s);
    kernaux_arch_i386_hang();
}

void halt()
{
    panic("Kernel main function returned.");
}
