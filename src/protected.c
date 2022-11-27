#include "protected.h"

#include "config.h"
#include "info.h"
#include "interrupts/main.h"

#include <kernaux/arch/i386.h>
#include <kernaux/asm/i386.h>
#include <kernaux/drivers/console.h>
#include <kernaux/drivers/intel_8259_pic.h>

#include <stdint.h>
#include <string.h>

struct GdtPointer {
    uint16_t limit;
    uint32_t base;
}
__attribute__((packed));

struct IdtPointer {
    uint16_t limit;
    uint32_t base;
}
__attribute__((packed));

static struct GdtPointer gdt_pointer;
static struct IdtPointer idt_pointer;

static struct KernAux_Arch_I386_DTE  gdt_entries[GDT_SIZE];
static struct KernAux_Arch_I386_IDTE idt_entries[IDT_SIZE];

static struct KernAux_Arch_I386_TSS tss;

static void gdt_set_gates();
static void idt_set_gates();

static void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);

void protected_initialize(const struct Kernel_Info *const kinfo)
{
    kernaux_drivers_intel_8259_pic_remap(32, 40);
    kernaux_drivers_intel_8259_pic_disable_all();

    kernaux_drivers_console_print("[INFO] protected: Setup GDT.\n");
    gdt_set_gates();

    kernaux_drivers_console_print("[INFO] protected: Setup IDT.\n");
    idt_set_gates();

    kernaux_drivers_console_print("[INFO] protected: Setup TSS.\n");
    memset(&tss, 0, sizeof(tss));
    tss.ss0 = GDT_KERNEL_DS_SELECTOR;
    tss.esp0 = kinfo->kernel_stack_start + kinfo->kernel_stack_size;

    kernaux_drivers_console_print("[INFO] protected: Load GDT.\n");
    gdt_pointer.limit = sizeof(struct KernAux_Arch_I386_DTE) * GDT_SIZE - 1;
    gdt_pointer.base  = (uint32_t)&gdt_entries;
    kernaux_asm_i386_flush_gdt(
        (uint32_t)&gdt_pointer,
        GDT_KERNEL_DS_SELECTOR,
        GDT_KERNEL_CS_SELECTOR
    );

    kernaux_drivers_console_print("[INFO] protected: Load IDT.\n");
    idt_pointer.limit = sizeof(struct KernAux_Arch_I386_IDTE) * IDT_SIZE - 1;
    idt_pointer.base  = (uint32_t)&idt_entries;
    kernaux_asm_i386_flush_idt((uint32_t)&idt_pointer);

    // kernaux_drivers_console_print("[INFO] protected: Load TSS.\n");
    // kernaux_asm_i386_flush_tss(GDT_TSS_SELECTOR);

    kernaux_drivers_console_print("[INFO] protected: Enable interrupts.\n");
    asm volatile ("sti");

    kernaux_drivers_console_print("[INFO] protected: Finished.\n");
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
        .always_1               = 1, // was 0
        .DPL                    = 3,
        .present                = 1,
    };
}

void idt_set_gates()
{
    memset(idt_entries, 0, sizeof(idt_entries));

    // exception
    idt_set_gate(0,  (uint32_t)interrupt_0,  0x08, 0x8E);
    idt_set_gate(1,  (uint32_t)interrupt_1,  0x08, 0x8E);
    idt_set_gate(2,  (uint32_t)interrupt_2,  0x08, 0x8E);
    idt_set_gate(3,  (uint32_t)interrupt_3,  0x08, 0x8E);
    idt_set_gate(4,  (uint32_t)interrupt_4,  0x08, 0x8E);
    idt_set_gate(5,  (uint32_t)interrupt_5,  0x08, 0x8E);
    idt_set_gate(6,  (uint32_t)interrupt_6,  0x08, 0x8E);
    idt_set_gate(7,  (uint32_t)interrupt_7,  0x08, 0x8E);
    idt_set_gate(8,  (uint32_t)interrupt_8,  0x08, 0x8E);
    idt_set_gate(9,  (uint32_t)interrupt_9,  0x08, 0x8E);
    idt_set_gate(10, (uint32_t)interrupt_10, 0x08, 0x8E);
    idt_set_gate(11, (uint32_t)interrupt_11, 0x08, 0x8E);
    idt_set_gate(12, (uint32_t)interrupt_12, 0x08, 0x8E);
    idt_set_gate(13, (uint32_t)interrupt_13, 0x08, 0x8E);
    idt_set_gate(14, (uint32_t)interrupt_14, 0x08, 0x8E);
    idt_set_gate(15, (uint32_t)interrupt_15, 0x08, 0x8E);
    idt_set_gate(16, (uint32_t)interrupt_16, 0x08, 0x8E);
    idt_set_gate(17, (uint32_t)interrupt_17, 0x08, 0x8E);
    idt_set_gate(18, (uint32_t)interrupt_18, 0x08, 0x8E);
    idt_set_gate(19, (uint32_t)interrupt_19, 0x08, 0x8E);
    idt_set_gate(20, (uint32_t)interrupt_20, 0x08, 0x8E);
    idt_set_gate(21, (uint32_t)interrupt_21, 0x08, 0x8E);
    idt_set_gate(22, (uint32_t)interrupt_22, 0x08, 0x8E);
    idt_set_gate(23, (uint32_t)interrupt_23, 0x08, 0x8E);
    idt_set_gate(24, (uint32_t)interrupt_24, 0x08, 0x8E);
    idt_set_gate(25, (uint32_t)interrupt_25, 0x08, 0x8E);
    idt_set_gate(26, (uint32_t)interrupt_26, 0x08, 0x8E);
    idt_set_gate(27, (uint32_t)interrupt_27, 0x08, 0x8E);
    idt_set_gate(28, (uint32_t)interrupt_28, 0x08, 0x8E);
    idt_set_gate(29, (uint32_t)interrupt_29, 0x08, 0x8E);
    idt_set_gate(30, (uint32_t)interrupt_30, 0x08, 0x8E);
    idt_set_gate(31, (uint32_t)interrupt_31, 0x08, 0x8E);

    // hwint: master PIC
    idt_set_gate(32, (uint32_t)interrupt_32, 0x08, 0x8E);
    idt_set_gate(33, (uint32_t)interrupt_33, 0x08, 0x8E);
    idt_set_gate(34, (uint32_t)interrupt_34, 0x08, 0x8E);
    idt_set_gate(35, (uint32_t)interrupt_35, 0x08, 0x8E);
    idt_set_gate(36, (uint32_t)interrupt_36, 0x08, 0x8E);
    idt_set_gate(37, (uint32_t)interrupt_37, 0x08, 0x8E);
    idt_set_gate(38, (uint32_t)interrupt_38, 0x08, 0x8E);
    idt_set_gate(39, (uint32_t)interrupt_39, 0x08, 0x8E);

    // hwint: slave PIC
    idt_set_gate(40, (uint32_t)interrupt_40, 0x08, 0x8E);
    idt_set_gate(41, (uint32_t)interrupt_41, 0x08, 0x8E);
    idt_set_gate(42, (uint32_t)interrupt_42, 0x08, 0x8E);
    idt_set_gate(43, (uint32_t)interrupt_43, 0x08, 0x8E);
    idt_set_gate(44, (uint32_t)interrupt_44, 0x08, 0x8E);
    idt_set_gate(45, (uint32_t)interrupt_45, 0x08, 0x8E);
    idt_set_gate(46, (uint32_t)interrupt_46, 0x08, 0x8E);
    idt_set_gate(47, (uint32_t)interrupt_47, 0x08, 0x8E);

    // syscall
    idt_set_gate(INT_SYSCALL, (uint32_t)interrupt_0x80, 0x08, 0x8E | 0x60);
}

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
    idt_entries[num].offset_low  = base & 0xFFFF;
    idt_entries[num].offset_high = (base >> 16) & 0xFFFF;
    idt_entries[num].selector    = sel;
    idt_entries[num]._zero0      = 0;
    idt_entries[num].flags       = flags;
}
