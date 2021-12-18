#ifndef KERNEL_INCLUDED_PAGING
#define KERNEL_INCLUDED_PAGING 1

#include "config.h"
#include "info.h"

#include <kernaux/arch/i386.h>

#define PAGE_DIR_ALIGN 4096

#define PAGE_DIR_ADDR(base) ((base) >> 12)

typedef struct KernAux_Arch_I386_PageDirEntry PageDir_Entry;

typedef PageDir_Entry PageDir[PAGE_DIR_LENGTH] __attribute__((aligned((PAGE_DIR_ALIGN))));

void paging_enable();

void paging_clear();
void paging_identity();
void paging_mapkernel(const struct Kernel_Info *kinfo);
void paging_load();

#endif
