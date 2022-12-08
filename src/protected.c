#include "protected.h"

#include "config.h"
#include "info.h"
#include "interrupts/main.h"

#include <drivers/console.h>
#include <drivers/intel_8259_pic.h>
#include <kernaux/arch/i386.h>
#include <kernaux/asm/i386.h>

#include <stdint.h>
#include <string.h>

static struct KernAux_Arch_I386_DTR gdt_pointer;
static struct KernAux_Arch_I386_DTR idt_pointer;

static struct KernAux_Arch_I386_DTE  gdt_entries[GDT_SIZE];
static struct KernAux_Arch_I386_IDTE idt_entries[IDT_SIZE];

static struct KernAux_Arch_I386_TSS tss;

static void gdt_set_gates();
static void idt_set_gates();

void protected_initialize(const struct Kernel_Info *const kinfo)
{
    drivers_intel_8259_pic_remap(32, 40);
    drivers_intel_8259_pic_disable_all();

    drivers_console_puts("[INFO] protected: Setup GDT.");
    gdt_set_gates();

    drivers_console_puts("[INFO] protected: Setup IDT.");
    idt_set_gates();

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
    idt_pointer.size = sizeof(struct KernAux_Arch_I386_IDTE) * IDT_SIZE - 1;
    idt_pointer.offset  = (uint32_t)&idt_entries;
    kernaux_asm_i386_flush_idt((uint32_t)&idt_pointer);

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

#define init_intr(num, offset, sel, dpl) \
    KernAux_Arch_I386_IDTE_init_intr(&idt_entries[num], (uint32_t)offset, sel, dpl)

void idt_set_gates()
{
    memset(idt_entries, 0, sizeof(idt_entries));

    // exception
    init_intr(0,  interrupt_0,  0x08, 0);
    init_intr(1,  interrupt_1,  0x08, 0);
    init_intr(2,  interrupt_2,  0x08, 0);
    init_intr(3,  interrupt_3,  0x08, 0);
    init_intr(4,  interrupt_4,  0x08, 0);
    init_intr(5,  interrupt_5,  0x08, 0);
    init_intr(6,  interrupt_6,  0x08, 0);
    init_intr(7,  interrupt_7,  0x08, 0);
    init_intr(8,  interrupt_8,  0x08, 0);
    init_intr(9,  interrupt_9,  0x08, 0);
    init_intr(10, interrupt_10, 0x08, 0);
    init_intr(11, interrupt_11, 0x08, 0);
    init_intr(12, interrupt_12, 0x08, 0);
    init_intr(13, interrupt_13, 0x08, 0);
    init_intr(14, interrupt_14, 0x08, 0);
    init_intr(15, interrupt_15, 0x08, 0);
    init_intr(16, interrupt_16, 0x08, 0);
    init_intr(17, interrupt_17, 0x08, 0);
    init_intr(18, interrupt_18, 0x08, 0);
    init_intr(19, interrupt_19, 0x08, 0);
    init_intr(20, interrupt_20, 0x08, 0);
    init_intr(21, interrupt_21, 0x08, 0);
    init_intr(22, interrupt_22, 0x08, 0);
    init_intr(23, interrupt_23, 0x08, 0);
    init_intr(24, interrupt_24, 0x08, 0);
    init_intr(25, interrupt_25, 0x08, 0);
    init_intr(26, interrupt_26, 0x08, 0);
    init_intr(27, interrupt_27, 0x08, 0);
    init_intr(28, interrupt_28, 0x08, 0);
    init_intr(29, interrupt_29, 0x08, 0);
    init_intr(30, interrupt_30, 0x08, 0);
    init_intr(31, interrupt_31, 0x08, 0);

    // hwint: master PIC
    init_intr(32, interrupt_32, 0x08, 0);
    init_intr(33, interrupt_33, 0x08, 0);
    init_intr(34, interrupt_34, 0x08, 0);
    init_intr(35, interrupt_35, 0x08, 0);
    init_intr(36, interrupt_36, 0x08, 0);
    init_intr(37, interrupt_37, 0x08, 0);
    init_intr(38, interrupt_38, 0x08, 0);
    init_intr(39, interrupt_39, 0x08, 0);

    // hwint: slave PIC
    init_intr(40, interrupt_40, 0x08, 0);
    init_intr(41, interrupt_41, 0x08, 0);
    init_intr(42, interrupt_42, 0x08, 0);
    init_intr(43, interrupt_43, 0x08, 0);
    init_intr(44, interrupt_44, 0x08, 0);
    init_intr(45, interrupt_45, 0x08, 0);
    init_intr(46, interrupt_46, 0x08, 0);
    init_intr(47, interrupt_47, 0x08, 0);

    // syscall
    init_intr(INT_SYSCALL, interrupt_0x80, 0x08, 3);
}
