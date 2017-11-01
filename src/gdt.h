#ifndef TAILIX_KERNEL_INCLUDED_GDT
#define TAILIX_KERNEL_INCLUDED_GDT 1

#include <stdint.h>

struct GdtPointer {
    uint16_t limit;
    uint32_t base;
}
__attribute__((packed));

struct GdtEntry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_middle;
    uint8_t  access;
    uint8_t  granularity;
    uint8_t  base_high;
}
__attribute__((packed));

void gdt_initialize();

#endif
