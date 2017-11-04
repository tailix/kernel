#include "console.h"
#include "logger.h"
#include "protected.h"
#include "paging.h"
#include "timer.h"

static void on_timer();

void main()
{
    console_initialize();

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
