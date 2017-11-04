#include "console.h"
#include "logger.h"
#include "multiboot.h"
#include "memory.h"

#include <kernelmq/info.h>

static struct KernelMQ_Info kinfo;

void init(struct KernelMQ_Multiboot_Info multiboot_info)
{
    console_initialize();

    logger_info("Multiboot info:");

    print_multiboot_info(multiboot_info);

    logger_info("Virtual memory info:");

    print_memory_info();
}
