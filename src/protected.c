#include "protected.h"

#include "config.h"
#include "info.h"
#include "interrupts.h"

#include <drivers/console.h>
#include <drivers/intel_8259_pic.h>
#include <kernaux/arch/i386.h>
#include <kernaux/asm/i386.h>

#include <stdint.h>
#include <string.h>

static struct KernAux_Arch_I386_DTR gdt_pointer;
static struct KernAux_Arch_I386_DTE gdt_entries[GDT_SIZE];

static struct KernAux_Arch_I386_TSS tss;

static void gdt_set_gates();

void protected_initialize(const struct Kernel_Info *const kinfo)
{
    drivers_intel_8259_pic_remap(32, 40);
    drivers_intel_8259_pic_disable_all();

    drivers_console_puts("[INFO] protected: Setup GDT.");
    gdt_set_gates();

    drivers_console_puts("[INFO] protected: Setup IDT.");
    interrupts_setup(GDT_KERNEL_CS_SELECTOR);

    drivers_console_puts("[INFO] protected: Setup TSS.");
    memset(&tss, 0, sizeof(tss));
    tss.ss0 = GDT_KERNEL_DS_SELECTOR;
    tss.esp0 = kinfo->kernel_stack_start + kinfo->kernel_stack_size;

    drivers_console_puts("[INFO] protected: Load GDT.");
    gdt_pointer.size = sizeof(struct KernAux_Arch_I386_DTE) * GDT_SIZE - 1;
    gdt_pointer.offset  = (uint32_t)&gdt_entries;
    kernaux_asm_i386_flush_gdt(
        (uint32_t)&gdt_pointer,
        GDT_KERNEL_DS_SELECTOR,
        GDT_KERNEL_CS_SELECTOR
    );

    drivers_console_puts("[INFO] protected: Load IDT.");
    interrupts_load();

    drivers_console_puts("[INFO] protected: Load TSS.");
    kernaux_asm_i386_flush_tss(GDT_TSS_SELECTOR);

    drivers_console_puts("[INFO] protected: Enable interrupts.");
    asm volatile ("sti");

    drivers_console_puts("[INFO] protected: Finished.");
}

void gdt_set_gates()
{
    memset(gdt_entries, 0, sizeof(gdt_entries));

    gdt_entries[GDT_NULL_INDEX].always_1 = 1;

    gdt_entries[GDT_KERNEL_CS_INDEX] = (struct KernAux_Arch_I386_DTE){
        .base_low               = 0,
        .base_high              = 0,
        .limit_low              = 0xFFFF,
        .limit_high             = 0xF,
        .available              = 0,
        .always_0               = 0,
        .big                    = 1,
        .gran                   = 1,
        .accessed               = 0,
        .read_write             = 1,
        .conforming_expand_down = 0,
        .code                   = 1,
        .always_1               = 1,
        .DPL                    = 0,
        .present                = 1,
    };

    gdt_entries[GDT_KERNEL_DS_INDEX] = (struct KernAux_Arch_I386_DTE){
        .base_low               = 0,
        .base_high              = 0,
        .limit_low              = 0xFFFF,
        .limit_high             = 0xF,
        .available              = 0,
        .always_0               = 0,
        .big                    = 1,
        .gran                   = 1,
        .accessed               = 0,
        .read_write             = 1,
        .conforming_expand_down = 0,
        .code                   = 0,
        .always_1               = 1,
        .DPL                    = 0,
        .present                = 1,
    };

    gdt_entries[GDT_USER_CS_INDEX] = (struct KernAux_Arch_I386_DTE){
        .base_low               = 0,
        .base_high              = 0,
        .limit_low              = 0xFFFF,
        .limit_high             = 0xF,
        .available              = 0,
        .always_0               = 0,
        .big                    = 1,
        .gran                   = 1,
        .accessed               = 0,
        .read_write             = 1,
        .conforming_expand_down = 0,
        .code                   = 1,
        .always_1               = 1,
        .DPL                    = 3,
        .present                = 1,
    };

    gdt_entries[GDT_USER_DS_INDEX] = (struct KernAux_Arch_I386_DTE){
        .base_low               = 0,
        .base_high              = 0,
        .limit_low              = 0xFFFF,
        .limit_high             = 0xF,
        .available              = 0,
        .always_0               = 0,
        .big                    = 1,
        .gran                   = 1,
        .accessed               = 0,
        .read_write             = 1,
        .conforming_expand_down = 0,
        .code                   = 0,
        .always_1               = 1,
        .DPL                    = 3,
        .present                = 1,
    };

    uint32_t base = (uint32_t)&tss;
    uint32_t limit = sizeof(tss);

    gdt_entries[GDT_TSS_INDEX] = (struct KernAux_Arch_I386_DTE){
        .base_low               = base & 0xFFFFFF,
        .base_high              = (base & 0xFF000000) >> 24,
        .limit_low              = limit & 0xFFFF,
        .limit_high             = (limit & 0xF0000) >> 16,
        .available              = 0,
        .always_0               = 0,
        .big                    = 0,
        .gran                   = 0,
        .accessed               = 1,
        .read_write             = 0,
        .conforming_expand_down = 0,
        .code                   = 1,
        .always_1               = 0,
        .DPL                    = 0,
        .present                = 1,
    };
}
