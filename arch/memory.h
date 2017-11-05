#ifndef KERNELMQ_INCLUDED_MEMORY
#define KERNELMQ_INCLUDED_MEMORY 1

#include <kernelmq/info.h>

void memory_initialize(const struct KernelMQ_Info *kinfo);

unsigned long memory_alloc_page();
unsigned long memory_alloc_big_page();

void memory_free_page(unsigned long addr);
void memory_free_big_page(unsigned long addr);

#endif
