#include "multiboot.h"
#include "logger.h"
#include "protected.h"
#include "timer.h"

void main(struct KernelMQ_Multiboot_Info multiboot_info)
{
    logger_initialize();

    print_multiboot_info(multiboot_info);

    logger_info("Kernel initialization started.");

    protected_initialize();
    timer_initialize(50);

    logger_warn("Nothing to do.");
    logger_fail("Halt.");
}
