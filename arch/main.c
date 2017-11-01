#include "logger.h"
#include "gdt.h"
#include "idt.h"

void main()
{
    logger_initialize();
    logger_info("Kernel initialization started.");

    gdt_initialize();
    idt_initialize();

    asm volatile ("int $0x3");
    asm volatile ("int $0x4");

    logger_warn("Nothing to do.");
    logger_fail("Halt.");
}
