#ifndef KERNELMQ_INCLUDED_PFA
#define KERNELMQ_INCLUDED_PFA 1

#include <kernelmq/info.h>

void pfa_initialize(const struct KernelMQ_Info *kinfo);

unsigned long pfa_alloc_page();
unsigned long pfa_alloc_big_page();

void pfa_free_page(unsigned long addr);
void pfa_free_big_page(unsigned long addr);

#endif
