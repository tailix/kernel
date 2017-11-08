#include "multiboot.h"
#include "paging.h"

#include <kernelmq/info.h>
#include <kernelmq/stdlib.h>

// Defined in linker script
extern char _kernel_offset;
extern char _kernel_size;
extern char _kernel_phys_base;
extern char _kernel_virt_base;
extern char _kernel_stack_top;

static struct KernelMQ_Info kinfo;

const struct KernelMQ_Info *init(unsigned long multiboot_magic, unsigned long multiboot_info_base)
{
    if (multiboot_magic != MULTIBOOT_MAGIC) {
        return 0;
    }

    kmemset(&kinfo, 0, sizeof(struct KernelMQ_Info));

    if (!multiboot_parse(&kinfo, multiboot_info_base)) {
        return 0;
    }

    kinfo.kernel_offset = (unsigned long)&_kernel_offset;
    kinfo.kernel_size   = (unsigned long)&_kernel_size;

    kinfo.kernel_phys_base = (unsigned long)&_kernel_phys_base;
    kinfo.kernel_virt_base = (unsigned long)&_kernel_virt_base;

    kinfo.kernel_phys_limit = kinfo.kernel_phys_base + kinfo.kernel_size - 1;
    kinfo.kernel_virt_limit = kinfo.kernel_virt_base + kinfo.kernel_size - 1;

    kinfo.kernel_and_modules_total_size = kinfo.kernel_size + kinfo.modules_total_size;

    kinfo.kernel_stack_top = (unsigned long)&_kernel_stack_top;

    paging_clear();
    paging_identity();
    /* kinfo.freepde_start = */ paging_mapkernel(&kinfo);
    paging_load();

    paging_enable();

    return &kinfo;
}
