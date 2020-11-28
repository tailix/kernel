#include "timer.h"

#include "logger.h"

#include <kernaux/arch/i386.h>

void timer_initialize(unsigned int frequency)
{
    logger_info_from("timer", "Initialize timer.");

    const unsigned int divisor = 1193180 / frequency;

    const unsigned char l = divisor & 0xFF;
    const unsigned char h = (divisor >> 8) & 0xFF;

    kernaux_arch_i386_outportb(0x43, 0x36);
    kernaux_arch_i386_outportb(0x40, l);
    kernaux_arch_i386_outportb(0x40, h);
}

void timer_register_handler(timer_handler_t handler)
{
    hwint_register_handler(0, handler);
}

void timer_unregister_handler()
{
    hwint_unregister_handler(0);
}
