#include "panic.h"

#include <kernaux/arch/x86.h>
#include <kernaux/console.h>

void panic(const char *const s)
{
    kernaux_console_printf("[FAIL] panic: %s\n", s);
    kernaux_arch_x86_hang();
}

void halt()
{
    panic("Kernel main function returned.");
}
