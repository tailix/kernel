#ifndef TAILIX_KERNEL_INCLUDED_GDT
#define TAILIX_KERNEL_INCLUDED_GDT 1

struct GdtPointer {
    unsigned short limit;
    unsigned int   base;
}
__attribute__((packed));

struct GdtEntry {
    unsigned short limit_low;
    unsigned short base_low;
    unsigned char  base_middle;
    unsigned char  access;
    unsigned char  granularity;
    unsigned char  base_high;
}
__attribute__((packed));

void gdt_initialize();

#endif
