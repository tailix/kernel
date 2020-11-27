#include "panic.h"

#include "logger.h"

#include <kernaux/arch/i386.h>

void panic(const char *const s)
{
    logger_fail_from("panic", s);
    kernaux_arch_i386_hang();
}

void halt()
{
    panic("Kernel main function returned.");
}
