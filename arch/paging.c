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

// TODO: implement this
#define assert(n) if (n) {}
#define panic()   if (1) {}

static unsigned long pagedir[I386_VM_PT_ENTRIES] __attribute__((aligned(4096)));

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
