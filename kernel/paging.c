#include "paging.h"
#include "panic.h"

#include <kernaux/asm/i386.h>
#include <kernaux/libc.h>

static void mapping(struct Paging *paging, uint32_t virt, uint32_t phys);

void paging_load(struct Paging *const paging)
{
    uint32_t page_dir_phys = (uint32_t)&paging->page_dir;
    kernaux_asm_i386_write_cr3(page_dir_phys);
}

void paging_enable()
{
    uint32_t cr0 = kernaux_asm_i386_read_cr0();
    uint32_t cr4 = kernaux_asm_i386_read_cr4();

    assert(cr0 & KERNAUX_ARCH_I386_CR0_PE, "The boot loader should have put us in protected mode.");

    // First clear PG and PGE flag, as PGE must be enabled after PG.
    kernaux_asm_i386_write_cr0(cr0 & ~KERNAUX_ARCH_I386_CR0_PG);
    kernaux_asm_i386_write_cr4(cr4 & ~(KERNAUX_ARCH_I386_CR4_PGE | KERNAUX_ARCH_I386_CR4_PSE));

    cr0 = kernaux_asm_i386_read_cr0();
    cr4 = kernaux_asm_i386_read_cr4();

    // Our page table contains 4MB entries.
    // cr4 |= KERNAUX_ARCH_I386_CR4_PSE;

    kernaux_asm_i386_write_cr4(cr4);

    // First enable paging, then enable global page flag.
    cr0 |= KERNAUX_ARCH_I386_CR0_PG;

    kernaux_asm_i386_write_cr0(cr0);

    cr0 |= KERNAUX_ARCH_I386_CR0_WP;

    kernaux_asm_i386_write_cr0(cr0);
    kernaux_asm_i386_write_cr4(cr4);
}

void paging_clear(struct Paging *const paging)
{
    memset(paging, 0, sizeof(*paging));
}

void paging_identity(struct Paging *const paging)
{
    for (size_t i = 0; i < KERNAUX_ARCH_I386_PAGES_COUNT_MAX; ++i) {
        const size_t addr = i * KERNAUX_ARCH_I386_PAGE_SIZE;
        mapping(paging, addr, addr);
    }
}

void paging_mapkernel(
    struct Paging *const paging,
    const struct Kernel_Info *const kinfo
) {
    assert(!(kinfo->kernel_phys_base % KERNAUX_ARCH_I386_PAGE_SIZE), "Kernel physical address is not aligned.");
    assert(!(kinfo->kernel_virt_base % KERNAUX_ARCH_I386_PAGE_SIZE), "Kernel virtual address is not aligned.");

    size_t phys = kinfo->kernel_phys_base;
    size_t virt = kinfo->kernel_virt_base;
    size_t mapped = 0;

    while (mapped < kinfo->kernel_size) {
        mapping(paging, virt, phys);

        phys   += KERNAUX_ARCH_I386_PAGE_SIZE;
        virt   += KERNAUX_ARCH_I386_PAGE_SIZE;
        mapped += KERNAUX_ARCH_I386_PAGE_SIZE;
    }
}

void mapping(
    struct Paging *const paging,
    const uint32_t virt,
    const uint32_t phys
) {
    KERNAUX_NOTNULL_RETURN(paging);

    const size_t pde_index = KERNAUX_ARCH_I386_ADDR_TO_PDE_INDEX(virt);
    const size_t pte_index = KERNAUX_ARCH_I386_ADDR_TO_PTE_INDEX(virt);

    struct KernAux_Arch_I386_PageDir   *const page_dir   = &paging->page_dir;
    struct KernAux_Arch_I386_PageTable *const page_table = &paging->page_tables[pde_index];

    struct KernAux_Arch_I386_PDE *const pde = &page_dir->pdes[pde_index];
    struct KernAux_Arch_I386_PTE *const pte = &page_table->ptes[pte_index];

    if (!pde->present) {
        pde->addr = KERNAUX_ARCH_I386_ADDR_TO_PDE_ADDR(page_table);

        pde->available1     = 0;
        pde->page_size      = 0;
        pde->available0     = 0;
        pde->accessed       = 0;
        pde->cache_disabled = 0;
        pde->write_through  = 0;
        pde->user           = 0;
        pde->writable       = 1;
        pde->present        = 1;
    }

    pte->addr = KERNAUX_ARCH_I386_ADDR_TO_PTE_ADDR(phys);

    pte->available      = 1;
    pte->global         = 1;
    pte->attr_table     = 0;
    pte->dirty          = 0;
    pte->accessed       = 0;
    pte->cache_disabled = 0;
    pte->write_through  = 0;
    pte->user           = 0;
    pte->writable       = 1;
    pte->present        = 1;
}
