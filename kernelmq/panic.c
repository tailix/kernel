#include "panic.h"

#include "logger.h"

#include <kernaux/arch/x86.h>

void panic(const char *const s)
{
    logger_fail_from("panic", s);
    kernaux_arch_x86_hang();
}

void halt()
{
    panic("Kernel main function returned.");
}
