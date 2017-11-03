#include "protected.h"

#include "config.h"
#include "logger.h"
#include "asm.h"
#include "exception.h"
#include "hwint.h"

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
    logger_info("Remap the IRQ table.");

    outportb(0x20, 0x11);
    outportb(0xA0, 0x11);
    outportb(0x21, 0x20);
    outportb(0xA1, 0x28);
    outportb(0x21, 0x04);
    outportb(0xA1, 0x02);
    outportb(0x21, 0x01);
    outportb(0xA1, 0x01);
    outportb(0x21, 0x00);
    outportb(0xA1, 0x00);

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

    idt_set_gate(0,  (unsigned int)exception_0,  0x08, 0x8E);
    idt_set_gate(1,  (unsigned int)exception_1,  0x08, 0x8E);
    idt_set_gate(2,  (unsigned int)exception_2,  0x08, 0x8E);
    idt_set_gate(3,  (unsigned int)exception_3,  0x08, 0x8E);
    idt_set_gate(4,  (unsigned int)exception_4,  0x08, 0x8E);
    idt_set_gate(5,  (unsigned int)exception_5,  0x08, 0x8E);
    idt_set_gate(6,  (unsigned int)exception_6,  0x08, 0x8E);
    idt_set_gate(7,  (unsigned int)exception_7,  0x08, 0x8E);
    idt_set_gate(8,  (unsigned int)exception_8,  0x08, 0x8E);
    idt_set_gate(9,  (unsigned int)exception_9,  0x08, 0x8E);
    idt_set_gate(10, (unsigned int)exception_10, 0x08, 0x8E);
    idt_set_gate(11, (unsigned int)exception_11, 0x08, 0x8E);
    idt_set_gate(12, (unsigned int)exception_12, 0x08, 0x8E);
    idt_set_gate(13, (unsigned int)exception_13, 0x08, 0x8E);
    idt_set_gate(14, (unsigned int)exception_14, 0x08, 0x8E);
    idt_set_gate(15, (unsigned int)exception_15, 0x08, 0x8E);
    idt_set_gate(16, (unsigned int)exception_16, 0x08, 0x8E);
    idt_set_gate(17, (unsigned int)exception_17, 0x08, 0x8E);
    idt_set_gate(18, (unsigned int)exception_18, 0x08, 0x8E);
    idt_set_gate(19, (unsigned int)exception_19, 0x08, 0x8E);
    idt_set_gate(20, (unsigned int)exception_20, 0x08, 0x8E);
    idt_set_gate(21, (unsigned int)exception_21, 0x08, 0x8E);
    idt_set_gate(22, (unsigned int)exception_22, 0x08, 0x8E);
    idt_set_gate(23, (unsigned int)exception_23, 0x08, 0x8E);
    idt_set_gate(24, (unsigned int)exception_24, 0x08, 0x8E);
    idt_set_gate(25, (unsigned int)exception_25, 0x08, 0x8E);
    idt_set_gate(26, (unsigned int)exception_26, 0x08, 0x8E);
    idt_set_gate(27, (unsigned int)exception_27, 0x08, 0x8E);
    idt_set_gate(28, (unsigned int)exception_28, 0x08, 0x8E);
    idt_set_gate(29, (unsigned int)exception_29, 0x08, 0x8E);
    idt_set_gate(30, (unsigned int)exception_30, 0x08, 0x8E);
    idt_set_gate(31, (unsigned int)exception_31, 0x08, 0x8E);

    idt_set_gate(32, (unsigned int)hwint_0,  0x08, 0x8E);
    idt_set_gate(33, (unsigned int)hwint_1,  0x08, 0x8E);
    idt_set_gate(34, (unsigned int)hwint_2,  0x08, 0x8E);
    idt_set_gate(35, (unsigned int)hwint_3,  0x08, 0x8E);
    idt_set_gate(36, (unsigned int)hwint_4,  0x08, 0x8E);
    idt_set_gate(37, (unsigned int)hwint_5,  0x08, 0x8E);
    idt_set_gate(38, (unsigned int)hwint_6,  0x08, 0x8E);
    idt_set_gate(39, (unsigned int)hwint_7,  0x08, 0x8E);
    idt_set_gate(40, (unsigned int)hwint_8,  0x08, 0x8E);
    idt_set_gate(41, (unsigned int)hwint_9,  0x08, 0x8E);
    idt_set_gate(42, (unsigned int)hwint_10, 0x08, 0x8E);
    idt_set_gate(43, (unsigned int)hwint_11, 0x08, 0x8E);
    idt_set_gate(44, (unsigned int)hwint_12, 0x08, 0x8E);
    idt_set_gate(45, (unsigned int)hwint_13, 0x08, 0x8E);
    idt_set_gate(46, (unsigned int)hwint_14, 0x08, 0x8E);
    idt_set_gate(47, (unsigned int)hwint_15, 0x08, 0x8E);

    logger_info("Load GDT.");

    gdt_pointer.limit = sizeof(struct GdtEntry) * GDT_SIZE - 1;
    gdt_pointer.base  = (unsigned int)&gdt_entries;

    gdt_flush(&gdt_pointer);

    logger_info("Load IDT.");

    idt_pointer.limit = sizeof(struct IdtEntry) * IDT_SIZE - 1;
    idt_pointer.base  = (unsigned int)&idt_entries;

    idt_flush(&idt_pointer);

    logger_info("Enable interrupts.");

    asm volatile ("sti");
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
