#include "paging.h"
#include "panic.h"

#include <kernaux/libc.h>
#include <kernaux/stdlib.h>

static void mapping(struct Paging *paging, uint32_t virt, uint32_t phys);

void paging_load(struct Paging *const paging)
{
    uint32_t page_dir_phys = (uint32_t)&paging->page_dir;
    kernaux_arch_i386_write_cr3(page_dir_phys);
}

void paging_enable()
{
    uint32_t cr0 = kernaux_arch_i386_read_cr0();
    uint32_t cr4 = kernaux_arch_i386_read_cr4();

    assert(cr0 & KERNAUX_ARCH_I386_CR0_PE, "The boot loader should have put us in protected mode.");

    // First clear PG and PGE flag, as PGE must be enabled after PG.
    kernaux_arch_i386_write_cr0(cr0 & ~KERNAUX_ARCH_I386_CR0_PG);
    kernaux_arch_i386_write_cr4(cr4 & ~(KERNAUX_ARCH_I386_CR4_PGE | KERNAUX_ARCH_I386_CR4_PSE));

    cr0 = kernaux_arch_i386_read_cr0();
    cr4 = kernaux_arch_i386_read_cr4();

    // Our page table contains 4MB entries.
    cr4 |= KERNAUX_ARCH_I386_CR4_PSE;

    kernaux_arch_i386_write_cr4(cr4);

    // First enable paging, then enable global page flag.
    cr0 |= KERNAUX_ARCH_I386_CR0_PG;

    kernaux_arch_i386_write_cr0(cr0);

    cr0 |= KERNAUX_ARCH_I386_CR0_WP;

    kernaux_arch_i386_write_cr0(cr0);
    kernaux_arch_i386_write_cr4(cr4);
}

void paging_clear(struct Paging *const paging)
{
    memset(paging, 0, sizeof(*paging));
}

void paging_identity(struct Paging *const paging)
{
    for (size_t i = 0; i < PAGE_DIR_LENGTH; ++i) {
        const size_t addr = i * PAGE_BIG_SIZE;
        mapping(paging, addr, addr);
    }
}

void paging_mapkernel(
    struct Paging *const paging,
    const struct Kernel_Info *const kinfo
) {
    assert(!(kinfo->kernel_phys_base % PAGE_BIG_SIZE), "Kernel physical address is not aligned.");
    assert(!(kinfo->kernel_virt_base % PAGE_BIG_SIZE), "Kernel virtual address is not aligned.");

    size_t phys = kinfo->kernel_phys_base;
    size_t virt = kinfo->kernel_virt_base;
    size_t mapped = 0;

    while (mapped < kinfo->kernel_size) {
        mapping(paging, virt, phys);

        phys   += PAGE_BIG_SIZE;
        virt   += PAGE_BIG_SIZE;
        mapped += PAGE_BIG_SIZE;
    }
}

void mapping(
    struct Paging *const paging,
    const uint32_t virt,
    const uint32_t phys
) {
    KERNAUX_NOTNULL_RETURN(paging);

    const size_t pde_index = virt / PAGE_BIG_SIZE;
    struct KernAux_Arch_I386_PDE *const pde = &paging->page_dir.pdes[pde_index];

    if (!pde->present) {
        pde->addr = PAGE_DIR_ADDR(phys);

        pde->available1     = 0;
        pde->page_size      = 1;
        pde->available0     = 0;
        pde->accessed       = 0;
        pde->cache_disabled = 0;
        pde->write_through  = 0;
        pde->user           = 0;
        pde->writable       = 1;
        pde->present        = 1;
    }
}
