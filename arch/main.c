#include "console.h"
#include "logger.h"
#include "protected.h"
#include "paging.h"
#include "timer.h"
#include "kprintf.h"

#include <kernelmq/info.h>
#include <kernelmq/stdlib.h>

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

    for (unsigned int i = 0; i < kinfo.modules_count; ++i) {
        struct KernelMQ_Info_Module *module = &kinfo.modules[i];

        kprintf(
            "Module at 0x%x, size 0x%x, command line: %s\n",
            module->base,
            module->size,
            module->cmdline
        );
    }

    protected_initialize();

    // paging_initialize();

    timer_register_handler(on_timer);
    timer_initialize(50);

    logger_warn("Nothing to do.");
    logger_fail("Halt.");
}

void on_timer()
{
    logger_info("Timer tick.");
}
