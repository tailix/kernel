#ifndef KERNELMQ_INCLUDED_MULTIBOOT
#define KERNELMQ_INCLUDED_MULTIBOOT 1

#include <kernelmq/info.h>

#define MULTIBOOT_MAGIC 0x36d76289

unsigned char multiboot_parse(struct KernelMQ_Info *kinfo, unsigned long addr);

#endif
