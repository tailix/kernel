#include "paging.h"

#include "page_dir.h"
#include "panic.h"

#include "stdlib.h"

// CR0 bits
#define I386_CR0_PE 0x00000001 // Protected mode
#define I386_CR0_MP 0x00000002 // Monitor Coprocessor
#define I386_CR0_EM 0x00000004 // Emulate
#define I386_CR0_TS 0x00000008 // Task Switched
#define I386_CR0_ET 0x00000010 // Extension Type
#define I386_CR0_WP 0x00010000 // Enable paging
#define I386_CR0_PG 0x80000000 // Enable paging

// some CR4 bits
#define I386_CR4_VME 0x00000001 // Virtual 8086
#define I386_CR4_PVI 0x00000002 // Virtual ints
#define I386_CR4_TSD 0x00000004 // RDTSC privileged
#define I386_CR4_DE  0x00000008 // Debugging extensions
#define I386_CR4_PSE 0x00000010 // Page size extensions
#define I386_CR4_PAE 0x00000020 // Physical addr extens
#define I386_CR4_MCE 0x00000040 // Machine check enable
#define I386_CR4_PGE 0x00000080 // Global page flag enable

unsigned long read_cr0();
unsigned long read_cr4();

void write_cr0(volatile unsigned long);
void write_cr3(volatile unsigned long);
void write_cr4(volatile unsigned long);

static PageDir page_dir;

void paging_enable()
{
    unsigned long cr0 = read_cr0();
    unsigned long cr4 = read_cr4();

    assert(cr0 & I386_CR0_PE, "The boot loader should have put us in protected mode.");

    // First clear PG and PGE flag, as PGE must be enabled after PG.
    write_cr0(cr0 & ~I386_CR0_PG);
    write_cr4(cr4 & ~(I386_CR4_PGE | I386_CR4_PSE));

    cr0 = read_cr0();
    cr4 = read_cr4();

    // Our page table contains 4MB entries.
    cr4 |= I386_CR4_PSE;

    write_cr4(cr4);

    // First enable paging, then enable global page flag.
    cr0 |= I386_CR0_PG;

    write_cr0(cr0);

    cr0 |= I386_CR0_WP;

    write_cr0(cr0);
    write_cr4(cr4);
}

void paging_clear()
{
    kmemset(page_dir, 0, sizeof(page_dir));
}

void paging_identity()
{
    for (int i = 0; i < PAGE_DIR_LENGTH; ++i) {
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

int paging_mapkernel(const struct KernelMQ_Info *const kinfo)
{
    assert(!(kinfo->kernel_phys_base % PAGE_BIG_SIZE), "Kernel physical address is not aligned.");
    assert(!(kinfo->kernel_virt_base % PAGE_BIG_SIZE), "Kernel virtual address is not aligned.");

    int pde = kinfo->kernel_virt_base / PAGE_BIG_SIZE;

    unsigned long mapped = 0;
    unsigned long kern_phys = kinfo->kernel_phys_base;

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

    return pde;
}

unsigned long paging_load()
{
    unsigned long page_dir_phys = (unsigned long)page_dir;
    write_cr3(page_dir_phys);
    return page_dir_phys;
}
