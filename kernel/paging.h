#ifndef KERNEL_INCLUDED_PAGING
#define KERNEL_INCLUDED_PAGING 1

#include "config.h"
#include "info.h"

#define PAGE_DIR_ALIGN 4096

#define PAGE_DIR_ADDR(base) ((base) >> 12)

struct PageDir_Entry {
    unsigned present        : 1;
    unsigned writable       : 1;
    unsigned user           : 1;
    unsigned write_through  : 1;
    unsigned cache_disabled : 1;
    unsigned accessed       : 1;
    unsigned always_0       : 1;
    unsigned page_size      : 1;
    unsigned ignored        : 1;
    unsigned unused         : 3;
    unsigned addr           : 20;
}
__attribute__((packed));

typedef struct PageDir_Entry PageDir[PAGE_DIR_LENGTH] __attribute__((aligned((PAGE_DIR_ALIGN))));

void paging_enable();

void paging_clear();
void paging_identity();
void paging_mapkernel(const struct Kernel_Info *kinfo);
void paging_load();

#endif
