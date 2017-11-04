#include "multiboot.h"

#include <kernelmq/info.h>
#include <kernelmq/stdlib.h>

// Defined in linker script
extern char _kernel_offset;
extern char _kernel_phys_base;
extern char _kernel_virt_base;

static struct KernelMQ_Info kinfo;

const struct KernelMQ_Info *init(struct KernelMQ_Multiboot_Info multiboot_info)
{
    kmemset(&kinfo, 0, sizeof(struct KernelMQ_Info));

    kinfo.kernel_offset = (unsigned long)&_kernel_offset;

    kinfo.kernel_phys_base = (unsigned long)&_kernel_phys_base;
    kinfo.kernel_virt_base = (unsigned long)&_kernel_virt_base;

    return &kinfo;
}
