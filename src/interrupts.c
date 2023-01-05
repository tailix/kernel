#include <drivers/console.h>
#include <kernaux/arch/i386.h>
#include <kernaux/asm/i386.h>

#include <stdint.h>
#include <string.h>

#define IDT_SIZE 256

static struct KernAux_Arch_I386_DTR idt_pointer;
static struct KernAux_Arch_I386_IDTE idt[IDT_SIZE];

void interrupts_init()
{
    memset(&idt_pointer, 0, sizeof(idt_pointer));
    memset(&idt, 0, sizeof(idt));
}

void interrupts_load()
{
    idt_pointer.size = sizeof(struct KernAux_Arch_I386_IDTE) * IDT_SIZE - 1;
    idt_pointer.offset  = (uint32_t)&idt;
    kernaux_asm_i386_flush_idt((uint32_t)&idt_pointer);
}

void interrupts_handler()
{
    drivers_console_puts("INTERRUPT!!!!!");
}

#define init_intr(num, dpl) do { \
    void interrupts_cb_##num();        \
    KernAux_Arch_I386_IDTE_init_intr(  \
        &idt[num],                     \
        (uint32_t)interrupts_cb_##num, \
        kernel_cs_selector,            \
        dpl                            \
    );                                 \
} while (0)

void interrupts_setup(const uint16_t kernel_cs_selector)
{
    memset(idt, 0, sizeof(idt));

    // Protected mode exteptions

    init_intr(0x00, 0);
    init_intr(0x01, 0);
    init_intr(0x02, 0);
    init_intr(0x03, 0);
    init_intr(0x04, 0);
    init_intr(0x05, 0);
    init_intr(0x06, 0);
    init_intr(0x07, 0);
    init_intr(0x08, 0);
    init_intr(0x09, 0);
    init_intr(0x0a, 0);
    init_intr(0x0b, 0);
    init_intr(0x0c, 0);
    init_intr(0x0d, 0);
    init_intr(0x0e, 0);
    init_intr(0x0f, 0);
    init_intr(0x10, 0);

    init_intr(0x11, 0);
    init_intr(0x12, 0);
    init_intr(0x13, 0);
    init_intr(0x14, 0);
    init_intr(0x15, 0);
    init_intr(0x16, 0);
    init_intr(0x17, 0);
    init_intr(0x18, 0);
    init_intr(0x19, 0);
    init_intr(0x1a, 0);
    init_intr(0x1b, 0);
    init_intr(0x1c, 0);
    init_intr(0x1d, 0);
    init_intr(0x1e, 0);
    init_intr(0x1f, 0);

    init_intr(0x80, 0);
}
