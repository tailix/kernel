#include "panic.h"

#include "logger.h"

void hang();

void panic(const char *const s)
{
    logger_fail_from("panic", s);
    hang();
}

void halt()
{
    panic("Kernel main function returned.");
}
