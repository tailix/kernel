#include "panic.h"
#include "poweroff.h"

#include <kernaux/asm/i386.h>
#include <kernaux/drivers/console.h>
#include <kernaux/drivers/qemu.h>

void panic(const char *const s)
{
    kernaux_drivers_console_printf("[FAIL] panic: %s\n", s);
    poweroff();
}

void halt()
{
    panic("Kernel main function returned.");
}

void kernaux_assert_fn(
    const char *const file,
    const int line,
    const char *const str
) {
    kernaux_drivers_console_printf("[FAIL] assertion failed: %s:%u: \"%s\"\n",
                                   file, line, str);
    poweroff();
}
