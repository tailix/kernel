#include "timer.h"

#include "logger.h"
#include "asm.h"

void timer_initialize(unsigned int frequency)
{
    logger_info("Initialize timer.");

    outportb(0x43, 0x36);
    outportb(0x40, (1193180 / frequency) & 0xFF);
    outportb(0x40, (1193180 / frequency) >> 8);
}
