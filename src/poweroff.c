#include "poweroff.h"

#include <stdbool.h>

#include <kernaux/drivers/qemu.h>

bool poweroff_doing = false;

void poweroff()
{
    poweroff_doing = true;
    kernaux_drivers_qemu_poweroff();
    for (;;) asm volatile(POWEROFF_ASM);
}
