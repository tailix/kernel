#include "console.h"
#include "logger.h"
#include "protected.h"
#include "paging.h"
#include "timer.h"

#include <kernelmq/info.h>
#include <kernelmq/stdlib.h>

static struct KernelMQ_Info kinfo;

static void on_timer();

void main(const struct KernelMQ_Info *const kinfo_ptr)
{
    console_initialize();

    kmemset(&kinfo, 0, sizeof(struct KernelMQ_Info));

    if (!kinfo_ptr) {
        logger_fail("No kernel information. Halt.");
        return;
    }

    kinfo = *kinfo_ptr;

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
