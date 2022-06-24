#include "timer.h"

#include <kernaux/asm/i386.h>
#include <kernaux/drivers/console.h>
#include <kernaux/drivers/intel_8253_pit.h>

void timer_initialize(unsigned int frequency)
{
    kernaux_drivers_console_print("[INFO] timer: Initialize timer.\n");
    kernaux_drivers_intel_8253_pit_initialize(frequency);
}

void timer_register_handler(timer_handler_t handler)
{
    hwint_register_handler(0, handler);
}

void timer_unregister_handler()
{
    hwint_unregister_handler(0);
}
