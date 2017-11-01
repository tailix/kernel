#include "logger.h"
#include "gdt.h"
#include "idt.h"

void main(uint32_t multiboot_magic)
{
    logger_initialize();

    if (multiboot_magic == 0x2BADB002) {
        logger_info("Loaded with Multiboot-compliant bootloader, specification version 1.");
    }
    else if (multiboot_magic == 0x36d76289) {
        logger_info("Loaded with Multiboot-compliant bootloader, specification version 2.");
    }
    else {
        logger_warn("Loaded with no Multiboot-compliant bootloader.");
    }

    logger_info("Kernel initialization started.");

    gdt_initialize();
    idt_initialize();

    asm volatile ("int $0x3");
    asm volatile ("int $0x4");

    logger_warn("Nothing to do.");
    logger_fail("Halt.");
}
