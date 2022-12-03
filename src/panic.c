#include "panic.h"

#include <drivers/console.h>
#include <drivers/shutdown.h>
#include <kernaux/asm/i386.h>

void panic(const char *const s)
{
    drivers_console_printf("[FAIL] panic: %s\n", s);
    drivers_shutdown_poweroff();
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
    drivers_console_printf("[FAIL] assertion failed: %s:%u: \"%s\"\n",
                           file, line, str);
    drivers_shutdown_poweroff();
}
