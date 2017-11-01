#ifndef TAILIX_KERNEL_INCLUDED_IDT
#define TAILIX_KERNEL_INCLUDED_IDT 1

struct IdtPointer {
    unsigned short limit;
    unsigned int   base;
}
__attribute__((packed));

struct IdtEntry {
    unsigned short base_lo;
    unsigned short sel;
    unsigned char  always0;
    unsigned char  flags;
    unsigned short base_hi;
}
__attribute__((packed));

void idt_initialize();

#endif
