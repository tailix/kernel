#include "console.h"
#include "logger.h"
#include "kprintf.h"
#include "multiboot.h"

#include <kernelmq/info.h>
#include <kernelmq/stdlib.h>

// Defined in linker script
extern char _kernel_offset;
extern char _kernel_phys_base;
extern char _kernel_virt_base;

static struct KernelMQ_Info kinfo;

void init(struct KernelMQ_Multiboot_Info multiboot_info)
{
    kmemset(&kinfo, 0, sizeof(struct KernelMQ_Info));

    kinfo.kernel_offset = (unsigned long)&_kernel_offset;

    kinfo.kernel_phys_base = (unsigned long)&_kernel_phys_base;
    kinfo.kernel_virt_base = (unsigned long)&_kernel_virt_base;

    console_initialize();

    logger_info("Multiboot info:");

    print_multiboot_info(multiboot_info);

    logger_info("Virtual memory info:");

    kprintf(
        "0x%x (phys base) + 0x%x (offset) = 0x%x (virt base)\n",
        kinfo.kernel_phys_base,
        kinfo.kernel_offset,
        kinfo.kernel_virt_base
    );
}
