#ifndef KERNELMQ_INCLUDED_MULTIBOOT
#define KERNELMQ_INCLUDED_MULTIBOOT 1

#include "info.h"

unsigned char multiboot_parse(struct KernelMQ_Info *kinfo, unsigned long base);

#endif
