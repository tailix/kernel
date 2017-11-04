#include "paging.h"

#include <kernelmq/stdlib.h>

#define I386_VM_PT_ENTRIES 1024

#define I386_PAGE_SIZE      4096
#define I386_BIG_PAGE_SIZE  (I386_PAGE_SIZE * I386_VM_PT_ENTRIES)

// i386 paging constants
#define I386_VM_PRESENT 0x001   // Page is present
#define I386_VM_WRITE   0x002   // Read/write access allowed
#define I386_VM_READ    0x000   // Read access only
#define I386_VM_USER    0x004   // User access allowed
#define I386_VM_PWT 0x008   // Write through
#define I386_VM_PCD 0x010   // Cache disable
#define I386_VM_ACC 0x020   // Accessed
#define I386_VM_ADDR_MASK 0xFFFFF000 // physical address
#define I386_VM_ADDR_MASK_4MB 0xFFC00000 // physical address
#define I386_VM_OFFSET_MASK_4MB 0x003FFFFF // physical address

// Page directory specific flags.
#define I386_VM_BIGPAGE 0x080 // 4MB page

// CR0 bits
#define I386_CR0_PE     0x00000001  // Protected mode
#define I386_CR0_MP     0x00000002  // Monitor Coprocessor
#define I386_CR0_EM     0x00000004  // Emulate
#define I386_CR0_TS     0x00000008  // Task Switched
#define I386_CR0_ET     0x00000010  // Extension Type
#define I386_CR0_WP     0x00010000  // Enable paging
#define I386_CR0_PG     0x80000000  // Enable paging

// some CR4 bits
#define I386_CR4_VME        0x00000001  // Virtual 8086
#define I386_CR4_PVI        0x00000002  // Virtual ints
#define I386_CR4_TSD        0x00000004  // RDTSC privileged
#define I386_CR4_DE     0x00000008  // Debugging extensions
#define I386_CR4_PSE        0x00000010  // Page size extensions
#define I386_CR4_PAE        0x00000020  // Physical addr extens
#define I386_CR4_MCE        0x00000040  // Machine check enable
#define I386_CR4_PGE    0x00000080  // Global page flag enable

// TODO: implement this
#define assert(n) if (n) {}
#define panic()   if (1) {}

unsigned long read_cr0();
unsigned long read_cr4();

void write_cr0(volatile unsigned long);
void write_cr3(volatile unsigned long);
void write_cr4(volatile unsigned long);

static unsigned long pagedir[I386_VM_PT_ENTRIES] __attribute__((aligned(4096)));

void paging_enable()
{
    unsigned long cr0 = read_cr0();
    unsigned long cr4 = read_cr4();

    // The boot loader should have put us in protected mode.
    assert(cr0 & I386_CR0_PE);

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

void paging_identity(const struct KernelMQ_Info *const kinfo)
{
    assert(kinfo->mem_upper_base);

    for (int i = 0; i < I386_VM_PT_ENTRIES; ++i) {
        unsigned int flags = I386_VM_PRESENT |
                             I386_VM_BIGPAGE |
                             I386_VM_USER    |
                             I386_VM_WRITE;

        unsigned long phys = i * I386_BIG_PAGE_SIZE;

        if ((kinfo->mem_upper_base & I386_VM_ADDR_MASK_4MB) <=
            (phys & I386_VM_ADDR_MASK_4MB)
        ) {
            flags |= I386_VM_PWT | I386_VM_PCD;
        }

        pagedir[i] = phys | flags;
    }
}

int paging_mapkernel(const struct KernelMQ_Info *const kinfo)
{
    assert(!(kinfo->kernel_phys_base % I386_BIG_PAGE_SIZE));
    assert(!(kinfo->kernel_virt_base % I386_BIG_PAGE_SIZE));

    int pde = kinfo->kernel_virt_base / I386_BIG_PAGE_SIZE;

    unsigned long mapped = 0;
    unsigned long kern_phys = kinfo->kernel_phys_base;

    while (mapped < kinfo->kernel_size) {
        pagedir[pde] = kern_phys | I386_VM_PRESENT |
                                   I386_VM_BIGPAGE |
                                   I386_VM_WRITE;

        mapped += I386_BIG_PAGE_SIZE;
        kern_phys += I386_BIG_PAGE_SIZE;

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
