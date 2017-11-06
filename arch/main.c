#include "console.h"
#include "logger.h"
#include "memory.h"
#include "protected.h"
#include "paging.h"
#include "timer.h"
#include "keyboard.h"
#include "tasks.h"

#include <kernelmq/info.h>
#include <kernelmq/stdlib.h>
#include <kernelmq/module.h>

static struct KernelMQ_Info kinfo;

#define TIMER_FREQ 50

static unsigned char timer_enabled = 1;
static unsigned long timer_ticks = 0;

static void on_timer();
static void on_keyboard(char c);

void main(const struct KernelMQ_Info *const kinfo_ptr)
{
    console_initialize();

    kmemset(&kinfo, 0, sizeof(struct KernelMQ_Info));

    if (!kernelmq_info_validate_and_copy(&kinfo, kinfo_ptr)) {
        logger_fail_from("main", "Invalid kernel information. Halt.");
        return;
    }

    logger_info_from("main", "Kernel command line: %s", kinfo.cmdline);

    logger_info_from(
        "main",
        "Kernel phys base 0x%x, limit 0x%x",
        kinfo.kernel_phys_base,
        kinfo.kernel_phys_limit
    );

    logger_info_from(
        "main",
        "Kernel virt base 0x%x, limit 0x%x",
        kinfo.kernel_virt_base,
        kinfo.kernel_virt_limit
    );

    logger_info_from(
        "main",
        "Kernel size 0x%x, offset 0x%x",
        kinfo.kernel_size,
        kinfo.kernel_offset
    );

    for (unsigned int i = 0; i < kinfo.modules_count; ++i) {
        struct KernelMQ_Info_Module *module = &kinfo.modules[i];

        logger_info_from(
            "main",
            "Module at 0x%x, size 0x%x, command line: %s",
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

    keyboard_register_handler(on_keyboard);

    timer_initialize(TIMER_FREQ);
    timer_register_handler(on_timer);

    while (timer_enabled) {}

    for (unsigned int i = 0; i < kinfo.modules_count; ++i) {
        tasks_switch_to_user(kinfo.modules[i].base);
    }

    logger_warn_from("main", "Nothing to do.");
    logger_fail_from("main", "Halt.");
}

void on_timer()
{
    if (timer_ticks % TIMER_FREQ == 0) {
        logger_info_from("main", "Timer tick %u.", timer_ticks);

        if (timer_ticks >= TIMER_FREQ * 3) {
            timer_unregister_handler();
            timer_enabled = 0;
        }
    }

    ++timer_ticks;
}

void on_keyboard(char c)
{
    logger_info_from("main", "Key pressed: '%c'.", c);
}
