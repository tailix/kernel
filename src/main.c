#include "logger.h"
#include "gdt.h"

void main()
{
    logger_initialize();
    logger_info("Kernel initialization started.");

    logger_info("GDT initialization started.");
    gdt_initialize();
    logger_info("GDT initialization finished.");

    logger_warn("Nothing to do.");
    logger_fail("Halt.");
}
