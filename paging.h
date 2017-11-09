#ifndef KERNELMQ_INCLUDED_PAGING
#define KERNELMQ_INCLUDED_PAGING 1

#include "info.h"

void paging_enable();

void paging_clear();
void paging_identity();
int paging_mapkernel(const struct KernelMQ_Info *kinfo);
unsigned long paging_load();

#endif
