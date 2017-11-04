#include "multiboot.h"

#include <kernelmq/info.h>
#include <kernelmq/stdlib.h>

// Defined in linker script
extern char _kernel_offset;
extern char _kernel_phys_base;
extern char _kernel_virt_base;

static struct KernelMQ_Info kinfo;

const struct KernelMQ_Info *init(unsigned long multiboot_magic, unsigned long multiboot_info_addr)
{
    if (multiboot_magic != MULTIBOOT_MAGIC) {
        return 0;
    }

    kmemset(&kinfo, 0, sizeof(struct KernelMQ_Info));

    if (!multiboot_parse(&kinfo, multiboot_info_addr)) {
        return 0;
    }

    kinfo.kernel_offset = (unsigned long)&_kernel_offset;

    kinfo.kernel_phys_base = (unsigned long)&_kernel_phys_base;
    kinfo.kernel_virt_base = (unsigned long)&_kernel_virt_base;

    return &kinfo;
}
