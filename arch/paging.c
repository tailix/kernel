#include "paging.h"

#include "config.h"
#include "panic.h"

#include <kernelmq/stdlib.h>

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

struct entry {
    unsigned int present        : 1;
    unsigned int writable       : 1;
    unsigned int user           : 1;
    unsigned int write_through  : 1;
    unsigned int cache_disabled : 1;
    unsigned int accessed       : 1;
    unsigned int always_0       : 1;
    unsigned int page_size      : 1;
    unsigned int ignored        : 1;
    unsigned int unused         : 3;
    unsigned int addr           : 20;
}
__attribute__((packed));

unsigned long read_cr0();
unsigned long read_cr4();

void write_cr0(volatile unsigned long);
void write_cr3(volatile unsigned long);
void write_cr4(volatile unsigned long);

static unsigned long pagedir[PAGE_DIR_SIZE] __attribute__((aligned(4096)));

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
    kmemset(pagedir, 0, sizeof(pagedir));
}

void paging_identity()
{
    for (int i = 0; i < PAGE_DIR_SIZE; ++i) {
        struct entry entry;

        entry.addr = (i * PAGE_BIG_SIZE) >> 12;

        entry.unused         = 0;
        entry.ignored        = 0;
        entry.page_size      = 1;
        entry.always_0       = 0;
        entry.accessed       = 0;
        entry.cache_disabled = 1;
        entry.write_through  = 1;
        entry.user           = 1;
        entry.writable       = 1;
        entry.present        = 1;

        pagedir[i] = *((unsigned long*)(&entry));
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
        struct entry entry;

        entry.addr = kern_phys >> 12;

        entry.unused         = 0;
        entry.ignored        = 0;
        entry.page_size      = 1;
        entry.always_0       = 0;
        entry.accessed       = 0;
        entry.cache_disabled = 0;
        entry.write_through  = 0;
        entry.user           = 0;
        entry.writable       = 1;
        entry.present        = 1;

        pagedir[pde] = *((unsigned long*)(&entry));

        mapped += PAGE_BIG_SIZE;
        kern_phys += PAGE_BIG_SIZE;

        ++pde;
    }

    return pde;
}

unsigned long paging_load()
{
    unsigned long pagedir_phys = (unsigned long)pagedir;
    write_cr3(pagedir_phys);
    return pagedir_phys;
}
