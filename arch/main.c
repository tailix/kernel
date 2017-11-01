#include <kernelmq/multiboot.h>

#include "logger.h"
#include "multiboot2.h"
#include "gdt.h"
#include "idt.h"

void main(unsigned int multiboot_magic, unsigned long multiboot_info)
{
    logger_initialize();

    if (multiboot_magic == KERNELMQ_MULTIBOOT_1_MAGIC) {
        logger_info("Loaded with Multiboot-compliant bootloader, specification version 1.");
    }
    else if (multiboot_magic == KERNELMQ_MULTIBOOT_2_MAGIC) {
        logger_info("Loaded with Multiboot-compliant bootloader, specification version 2.");
        print_multiboot2_info(multiboot_info);
    }
    else {
        logger_warn("Loaded with no Multiboot-compliant bootloader.");
    }

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
