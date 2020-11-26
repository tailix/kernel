#include "panic.h"
#include "protected.h"
#include "paging.h"

#include "info.h"
#include "stdlib.h"
#include "module.h"
#include "process.h"

#include "tasks.h"
#include "elf.h"
#include "logger.h"

static struct KernelMQ_Info kinfo;

static KernelMQ_Process_List process_list;

void init(const struct KernelMQ_Info *const kinfo_ptr)
{
    kmemset(&kinfo, 0, sizeof(struct KernelMQ_Info));

    assert(kernelmq_info_validate_and_copy(&kinfo, kinfo_ptr), "Invalid kernel information.");

    protected_initialize(&kinfo);

    // Set up a new post-relocate bootstrap pagetable so that
    // we can map in VM, and we no longer rely on pre-relocated
    // data.
    paging_clear();
    paging_identity(); // Still need 1:1 for lapic and video mem and such.
    paging_mapkernel(&kinfo);
    paging_load();

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
