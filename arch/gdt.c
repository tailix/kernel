#include "gdt.h"

#include "logger.h"

#define GDT_SIZE 3

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

static struct GdtPointer gdt_pointer;

static struct GdtEntry gdt_entries[GDT_SIZE];

static void gdt_set_gate(int num, unsigned int base, unsigned int limit, unsigned char access, unsigned char gran);

void gdt_flush(unsigned int pointer);

void gdt_initialize()
{
    logger_info("Setup GDT.");

    gdt_set_gate(0, 0, 0, 0, 0);                // Null segment
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment

    logger_info("Load GDT.");

    gdt_pointer.limit = sizeof(struct GdtEntry) * GDT_SIZE - 1;
    gdt_pointer.base  = (unsigned int)&gdt_entries;

    gdt_flush((unsigned int)&gdt_pointer);
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
