#include "timer.h"

#include <drivers/console.h>
#include <drivers/intel_8253_pit.h>
#include <kernaux/asm/i386.h>

void timer_initialize(unsigned int frequency)
{
    drivers_console_puts("[INFO] timer: Initialize timer.");
    drivers_intel_8253_pit_initialize(frequency);
}

void timer_register_handler(timer_handler_t handler)
{
    hwint_register_handler(0, handler);
}

void timer_unregister_handler()
{
    hwint_unregister_handler(0);
}
