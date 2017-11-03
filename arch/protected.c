#include "protected.h"

#include "config.h"
#include "logger.h"
#include "exception.h"

struct GdtPointer {
    unsigned short limit;
    unsigned int   base;
}
__attribute__((packed));

struct IdtPointer {
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

struct IdtEntry {
    unsigned short base_lo;
    unsigned short sel;
    unsigned char  always0;
    unsigned char  flags;
    unsigned short base_hi;
}
__attribute__((packed));

static struct GdtPointer gdt_pointer;
static struct IdtPointer idt_pointer;

static struct GdtEntry gdt_entries[GDT_SIZE];
static struct IdtEntry idt_entries[IDT_SIZE];

static void gdt_set_gate(int num, unsigned int base, unsigned int limit, unsigned char access, unsigned char gran);

static void idt_set_gate(unsigned char num, unsigned int base, unsigned short sel, unsigned char flags);

void gdt_flush(const struct GdtPointer *pointer);
void idt_flush(const struct IdtPointer *pointer);

void protected_initialize()
{
    logger_info("Setup GDT.");

    gdt_set_gate(GDT_NULL_INDEX,      0, 0x00000000, 0,    0);
    gdt_set_gate(GDT_KERNEL_CS_INDEX, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    gdt_set_gate(GDT_KERNEL_DS_INDEX, 0, 0xFFFFFFFF, 0x92, 0xCF);
    gdt_set_gate(GDT_USER_CS_INDEX,   0, 0xFFFFFFFF, 0xFA, 0xCF);
    gdt_set_gate(GDT_USER_DS_INDEX,   0, 0xFFFFFFFF, 0xF2, 0xCF);

    logger_info("Setup IDT.");

    for (unsigned char *p = (unsigned char*)idt_entries; p < (unsigned char*)&idt_entries[IDT_SIZE]; ++p) {
        *p = 0;
    }

    idt_set_gate(0,  (unsigned int)isr0,  0x08, 0x8E);
    idt_set_gate(1,  (unsigned int)isr1,  0x08, 0x8E);
    idt_set_gate(2,  (unsigned int)isr2,  0x08, 0x8E);
    idt_set_gate(3,  (unsigned int)isr3,  0x08, 0x8E);
    idt_set_gate(4,  (unsigned int)isr4,  0x08, 0x8E);
    idt_set_gate(5,  (unsigned int)isr5,  0x08, 0x8E);
    idt_set_gate(6,  (unsigned int)isr6,  0x08, 0x8E);
    idt_set_gate(7,  (unsigned int)isr7,  0x08, 0x8E);
    idt_set_gate(8,  (unsigned int)isr8,  0x08, 0x8E);
    idt_set_gate(9,  (unsigned int)isr9,  0x08, 0x8E);
    idt_set_gate(10, (unsigned int)isr10, 0x08, 0x8E);
    idt_set_gate(11, (unsigned int)isr11, 0x08, 0x8E);
    idt_set_gate(12, (unsigned int)isr12, 0x08, 0x8E);
    idt_set_gate(13, (unsigned int)isr13, 0x08, 0x8E);
    idt_set_gate(14, (unsigned int)isr14, 0x08, 0x8E);
    idt_set_gate(15, (unsigned int)isr15, 0x08, 0x8E);
    idt_set_gate(16, (unsigned int)isr16, 0x08, 0x8E);
    idt_set_gate(17, (unsigned int)isr17, 0x08, 0x8E);
    idt_set_gate(18, (unsigned int)isr18, 0x08, 0x8E);
    idt_set_gate(19, (unsigned int)isr19, 0x08, 0x8E);
    idt_set_gate(20, (unsigned int)isr20, 0x08, 0x8E);
    idt_set_gate(21, (unsigned int)isr21, 0x08, 0x8E);
    idt_set_gate(22, (unsigned int)isr22, 0x08, 0x8E);
    idt_set_gate(23, (unsigned int)isr23, 0x08, 0x8E);
    idt_set_gate(24, (unsigned int)isr24, 0x08, 0x8E);
    idt_set_gate(25, (unsigned int)isr25, 0x08, 0x8E);
    idt_set_gate(26, (unsigned int)isr26, 0x08, 0x8E);
    idt_set_gate(27, (unsigned int)isr27, 0x08, 0x8E);
    idt_set_gate(28, (unsigned int)isr28, 0x08, 0x8E);
    idt_set_gate(29, (unsigned int)isr29, 0x08, 0x8E);
    idt_set_gate(30, (unsigned int)isr30, 0x08, 0x8E);
    idt_set_gate(31, (unsigned int)isr31, 0x08, 0x8E);

    logger_info("Load GDT.");

    gdt_pointer.limit = sizeof(struct GdtEntry) * GDT_SIZE - 1;
    gdt_pointer.base  = (unsigned int)&gdt_entries;

    gdt_flush(&gdt_pointer);

    logger_info("Load IDT.");

    idt_pointer.limit = sizeof(struct IdtEntry) * IDT_SIZE - 1;
    idt_pointer.base  = (unsigned int)&idt_entries;

    idt_flush(&idt_pointer);
}

void gdt_set_gate(int num, unsigned int base, unsigned int limit, unsigned char access, unsigned char gran)
{
    gdt_entries[num].base_low    = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high   = (base >> 24) & 0xFF;

    gdt_entries[num].limit_low   = (limit & 0xFFFF);
    gdt_entries[num].granularity = (limit >> 16) & 0x0F;

    gdt_entries[num].granularity |= gran & 0xF0;

    gdt_entries[num].access = access;
}

void idt_set_gate(unsigned char num, unsigned int base, unsigned short sel, unsigned char flags)
{
    idt_entries[num].base_lo = base & 0xFFFF;
    idt_entries[num].base_hi = (base >> 16) & 0xFFFF;
    idt_entries[num].sel     = sel;
    idt_entries[num].always0 = 0;
    // We must uncomment the OR below when we get to using user-mode.
    // It sets the interrupt gate's privilege level to 3.
    idt_entries[num].flags   = flags /* | 0x60 */;
}
