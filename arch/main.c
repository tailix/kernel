#include <kernelmq/multiboot.h>

#include "logger.h"
#include "gdt.h"
#include "idt.h"

void main(struct KernelMQ_Multiboot_Info multiboot_info)
{
    logger_initialize();

    print_multiboot_info(multiboot_info);

    logger_info("Kernel initialization started.");

    gdt_initialize();
    idt_initialize();

    asm volatile ("int $0x0");
    asm volatile ("int $0x3");
    asm volatile ("int $0x4");
    asm volatile ("int $0xF");
    asm volatile ("int $0x10");
    asm volatile ("int $0x1F");

    logger_warn("Nothing to do.");
    logger_fail("Halt.");
}
