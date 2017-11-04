#include "multiboot.h"
#include "logger.h"

void init(struct KernelMQ_Multiboot_Info multiboot_info)
{
    logger_initialize();

    print_multiboot_info(multiboot_info);
}
