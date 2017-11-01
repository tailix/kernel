#include "logger.h"
#include "gdt.h"

void main()
{
    logger_initialize();
    logger_info("Kernel initialization started.");

    gdt_initialize();

    logger_warn("Nothing to do.");
    logger_fail("Halt.");
}
