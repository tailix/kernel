#ifndef KERNELMQ_INCLUDED_PAGING
#define KERNELMQ_INCLUDED_PAGING 1

#include <kernelmq/info.h>

void paging_clear();
void paging_identity(const struct KernelMQ_Info *kinfo);
int paging_mapkernel(const struct KernelMQ_Info *kinfo);

#endif
