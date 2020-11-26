#include "multiboot.h"
#include "paging.h"

#include "info.h"
#include "stdlib.h"

#include "panic.h"
#include "protected.h"

#include "module.h"
#include "process.h"

#include "tasks.h"
#include "elf.h"
#include "logger.h"

// Defined in linker script
extern char _kernel_offset;
extern char _kernel_size;
extern char _kernel_phys_base;
extern char _kernel_virt_base;
extern char _kernel_stack_top;

static struct KernelMQ_Info kinfo;

static KernelMQ_Process_List process_list;

void main(unsigned long multiboot_magic, unsigned long multiboot_info_base)
{
    if (multiboot_magic != MULTIBOOT_MAGIC) {
        panic("Multiboot 2 magic number is invalid.");
    }

    kmemset(&kinfo, 0, sizeof(struct KernelMQ_Info));

    if (!multiboot_parse(&kinfo, multiboot_info_base)) {
        panic("Can not parse Multiboot 2 info.");
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

    assert(kernelmq_info_validate(&kinfo), "Invalid kernel information.");

    protected_initialize(&kinfo);

    const enum KernelMQ_Process_Error process_list_init_result =
        KernelMQ_Process_List_init(&process_list, &kinfo);

    if (process_list_init_result != KERNELMQ_PROCESS_ERROR_OK) {
        logger_fail_from(
            "init",
            "Process list initialization failed with %u.",
            process_list_init_result
        );

        panic("Can not initialize process list.");
    }

    logger_debug_from("init", "Process list initialized.");

    KernelMQ_Process_List_print(&process_list);

    if (kinfo.modules_count > 0) {
        const struct KernelMQ_ELF_Header *const elf_header =
            (void*)kinfo.modules[0].base;

        if (KernelMQ_ELF_Header_is_valid(elf_header)) {
            const unsigned long real_entrypoint =
                kinfo.modules[0].base + elf_header->entrypoint;

            tasks_switch_to_user(real_entrypoint);
        }
        else {
            logger_warn_from("init", "Invalid ELF header");
        }
    }
}
