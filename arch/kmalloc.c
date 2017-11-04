#include "kmalloc.h"

unsigned int kmalloc_placement_addr;

static unsigned int kmalloc_internal(unsigned int size, unsigned char align, unsigned int *phys);

unsigned int kmalloc(unsigned int size)
{
    return kmalloc_internal(size, 0, 0);
}

unsigned int kmalloc_a(unsigned int size)
{
    return kmalloc_internal(size, 1, 0);
}

unsigned int kmalloc_p(unsigned int size, unsigned int *phys)
{
    return kmalloc_internal(size, 0, phys);
}

unsigned int kmalloc_ap(unsigned int size, unsigned int *phys)
{
    return kmalloc_internal(size, 1, phys);
}

unsigned int kmalloc_internal(unsigned int size, unsigned char align, unsigned int *phys)
{
    if (align && (kmalloc_placement_addr & 0xFFFFF000)) {
        kmalloc_placement_addr &= 0xFFFFF000;
        kmalloc_placement_addr += 0x1000;
    }

    const unsigned int result = kmalloc_placement_addr;

    if (phys) {
        *phys = result;
    }

    kmalloc_placement_addr += size;

    return result;
}
