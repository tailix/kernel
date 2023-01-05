#ifndef KERNEL_INCLUDED_PAGING
#define KERNEL_INCLUDED_PAGING 1

#include "config.h"

#include <kernaux/arch/i386.h>

#define PAGE_DIR_ALIGN 4096

#define PAGE_DIR_ADDR(base) ((base) >> 12)

struct Paging {
    struct KernAux_Arch_I386_PageDir page_dir;
    // FIXME: use PFA to allocate page tables dynamically.
    struct KernAux_Arch_I386_PageTable page_tables[KERNAUX_ARCH_I386_PAGE_DIR_ENTRIES_COUNT];
}
__attribute__((packed))
__attribute__((aligned((PAGE_DIR_ALIGN))))
;

void paging_load(struct Paging *paging);
void paging_enable();

void paging_clear(struct Paging *paging);

void paging_identity(struct Paging *paging);
void paging_mapkernel(struct Paging *paging);

#endif
