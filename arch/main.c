#include <kernelmq/multiboot.h>

#include "logger.h"
#include "protected.h"

void main(struct KernelMQ_Multiboot_Info multiboot_info)
{
    logger_initialize();

    print_multiboot_info(multiboot_info);

    logger_info("Kernel initialization started.");

    gdt_initialize();
    idt_initialize();

    asm volatile ("int $0x3");
    asm volatile ("int $0x4");

    logger_warn("Nothing to do.");
    logger_fail("Halt.");
}
