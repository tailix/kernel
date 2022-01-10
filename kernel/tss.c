#include "tss.h"

#include "config.h"

#include <kernaux/arch/i386.h>
#include <kernaux/libc.h>

struct tss_entry {
    unsigned int prev_tss;
    unsigned int esp0;
    unsigned int ss0;
    unsigned int esp1;
    unsigned int ss1;
    unsigned int esp2;
    unsigned int ss2;
    unsigned int cr3;
    unsigned int eip;
    unsigned int eflags;
    unsigned int eax;
    unsigned int ecx;
    unsigned int edx;
    unsigned int ebx;
    unsigned int esp;
    unsigned int ebp;
    unsigned int esi;
    unsigned int edi;
    unsigned int es;
    unsigned int cs;
    unsigned int ss;
    unsigned int ds;
    unsigned int fs;
    unsigned int gs;
    unsigned int ldt;
    unsigned short trap;
    unsigned short iomap_base;
};

static struct tss_entry tss;

void tss_write_to_gdt(const struct Kernel_Info *const kinfo, void *gdt_entry_ptr)
{
    struct KernAux_Arch_I386_GDTE *const g = gdt_entry_ptr;

    unsigned long base = (unsigned long)&tss;
    unsigned long limit = sizeof(tss);

    g->limit_low = limit & 0xFFFF;
    g->base_low  = base  & 0xFFFFFF;

    g->base_high  = (base  & 0xFF000000) >> 24;
    g->limit_high = (limit & 0xF0000)    >> 16;

    g->accessed               = 1;
    g->read_write             = 0;
    g->conforming_expand_down = 0;
    g->code                   = 1;
    g->always_1               = 0;
    g->DPL                    = 3;
    g->present                = 1;
    g->available              = 0;
    g->always_0               = 0;
    g->big                    = 0;
    g->gran                   = 0;

    memset(&tss, 0, sizeof(tss));

    tss.ss0 = GDT_KERNEL_DS_SELECTOR;
    tss.esp0 = kinfo->kernel_stack_start + kinfo->kernel_stack_size;
}
