#ifndef TAILIX_KERNEL_INCLUDED_IDT
#define TAILIX_KERNEL_INCLUDED_IDT 1

#include <stdint.h>

struct IdtPointer {
    uint16_t limit;
    uint32_t base;
}
__attribute__((packed));

struct IdtEntry {
    uint16_t base_lo;
    uint16_t sel;
    uint8_t  always0;
    uint8_t  flags;
    uint16_t base_hi;
}
__attribute__((packed));

void idt_initialize();

#endif
