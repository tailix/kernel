#include "multiboot.h"
#include "console.h"

void init(struct KernelMQ_Multiboot_Info multiboot_info)
{
    console_initialize();

    print_multiboot_info(multiboot_info);
}
