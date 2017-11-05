#include "console.h"
#include "logger.h"
#include "memory.h"
#include "protected.h"
#include "paging.h"
#include "timer.h"
#include "kprintf.h"

#include <kernelmq/info.h>
#include <kernelmq/stdlib.h>
#include <kernelmq/module.h>

static struct KernelMQ_Info kinfo;

static void on_timer();

void main(const struct KernelMQ_Info *const kinfo_ptr)
{
    console_initialize();

    kmemset(&kinfo, 0, sizeof(struct KernelMQ_Info));

    if (!kernelmq_info_validate_and_copy(&kinfo, kinfo_ptr)) {
        logger_fail("Invalid kernel information. Halt.");
        return;
    }

    kprintf("Kernel command line: %s\n", kinfo.cmdline);

    kprintf(
        "Kernel phys base 0x%x, limit 0x%x\n",
        kinfo.kernel_phys_base,
        kinfo.kernel_phys_limit
    );

    kprintf(
        "Kernel virt base 0x%x, limit 0x%x\n",
        kinfo.kernel_virt_base,
        kinfo.kernel_virt_limit
    );

    kprintf(
        "Kernel size 0x%x, offset 0x%x\n",
        kinfo.kernel_size,
        kinfo.kernel_offset
    );

    for (unsigned int i = 0; i < kinfo.modules_count; ++i) {
        struct KernelMQ_Info_Module *module = &kinfo.modules[i];

        kprintf(
            "Module at 0x%x, size 0x%x, command line: %s\n",
            module->base,
            module->size,
            module->cmdline
        );
    }

    memory_initialize(&kinfo);

    protected_initialize(&kinfo);

    // Set up a new post-relocate bootstrap pagetable so that
    // we can map in VM, and we no longer rely on pre-relocated
    // data.
    paging_clear();
    paging_identity(); // Still need 1:1 for lapic and video mem and such.
    paging_mapkernel(&kinfo);
    paging_load();

    for (unsigned int i = 0; i < kinfo.modules_count; ++i) {
        ((KernelMQ_Module_Function)kinfo.modules[i].base)();
    }

    timer_register_handler(on_timer);
    timer_initialize(50);

    logger_warn("Nothing to do.");
    logger_fail("Halt.");
}

void on_timer()
{
    logger_info("Timer tick.");
}
