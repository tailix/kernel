#include "paging.h"

#include "panic.h"

#include <kernaux/arch/i386.h>
#include <kernaux/libc.h>
#include <kernaux/stdlib.h>

static PageDir page_dir;

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

void paging_clear()
{
    memset(page_dir, 0, sizeof(page_dir));
}

void paging_identity()
{
    for (size_t i = 0; i < PAGE_DIR_LENGTH; ++i) {
        page_dir[i].addr = PAGE_DIR_ADDR(i * PAGE_BIG_SIZE);

        page_dir[i].unused         = 0;
        page_dir[i].ignored        = 0;
        page_dir[i].page_size      = 1;
        page_dir[i].always_0       = 0;
        page_dir[i].accessed       = 0;
        page_dir[i].cache_disabled = 1;
        page_dir[i].write_through  = 1;
        page_dir[i].user           = 1;
        page_dir[i].writable       = 1;
        page_dir[i].present        = 1;
    }
}

void paging_mapkernel(const struct Kernel_Info *const kinfo)
{
    assert(!(kinfo->kernel_phys_base % PAGE_BIG_SIZE), "Kernel physical address is not aligned.");
    assert(!(kinfo->kernel_virt_base % PAGE_BIG_SIZE), "Kernel virtual address is not aligned.");

    size_t pde = kinfo->kernel_virt_base / PAGE_BIG_SIZE;

    size_t mapped = 0;
    size_t kern_phys = kinfo->kernel_phys_base;

    while (mapped < kinfo->kernel_size) {
        page_dir[pde].addr = PAGE_DIR_ADDR(kern_phys);

        page_dir[pde].unused         = 0;
        page_dir[pde].ignored        = 0;
        page_dir[pde].page_size      = 1;
        page_dir[pde].always_0       = 0;
        page_dir[pde].accessed       = 0;
        page_dir[pde].cache_disabled = 0;
        page_dir[pde].write_through  = 0;
        page_dir[pde].user           = 0;
        page_dir[pde].writable       = 1;
        page_dir[pde].present        = 1;

        mapped += PAGE_BIG_SIZE;
        kern_phys += PAGE_BIG_SIZE;

        ++pde;
    }
}

void paging_load()
{
    size_t page_dir_phys = (size_t)page_dir;
    kernaux_arch_i386_write_cr3(page_dir_phys);
}
