#ifndef KERNEL_INCLUDED_TSS
#define KERNEL_INCLUDED_TSS 1

#include "info.h"

void tss_write_to_gdt(const struct Kernel_Info *kinfo, void *gdt_entry);

void tss_flush();

#endif
