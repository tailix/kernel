#include "pic.h"

#include <kernaux/asm/i386.h>
#include <kernaux/console.h>

#define MASTER_COMMAND 0x20
#define MASTER_DATA    0x21

#define SLAVE_COMMAND 0xA0
#define SLAVE_DATA    0xA1

#define IRQS_COUNT 8
#define IRQS_TOTAL 16

static unsigned char master_irq_start = 0;
static unsigned char slave_irq_start  = 8;

void pic_enable_all()
{
    kernaux_console_print("[INFO] pic: Enable all IRQs.\n");

    kernaux_asm_i386_outportb(MASTER_DATA, 0);
    kernaux_asm_i386_outportb(SLAVE_DATA,  0);
}

void pic_disable_all()
{
    kernaux_console_print("[INFO] pic: Disable all IRQs.\n");

    kernaux_asm_i386_outportb(MASTER_DATA, 0xFF);
    kernaux_asm_i386_outportb(SLAVE_DATA,  0xFF);
}

void pic_enable(const unsigned char line)
{
    if (line >= IRQS_TOTAL) {
        kernaux_console_printf("[WARN] pic: Invalid line %u.\n", line);
        return;
    }

    kernaux_console_printf("[INFO] pic: Enable line %u.\n", line);

    if (line < IRQS_COUNT) {
        const unsigned char mask = kernaux_asm_i386_inportb(MASTER_DATA);
        kernaux_asm_i386_outportb(MASTER_DATA, mask & ~(1 << line));
    }
    else {
        const unsigned char mask = kernaux_asm_i386_inportb(SLAVE_DATA);
        kernaux_asm_i386_outportb(SLAVE_DATA, mask & ~(1 << (line - IRQS_COUNT)));
    }
}

void pic_disable(const unsigned char line)
{
    if (line >= IRQS_TOTAL) {
        kernaux_console_printf("[WARN] pic: Invalid line %u.\n", line);
        return;
    }

    kernaux_console_printf("[INFO] pic: Disable line %u.\n", line);

    if (line < IRQS_COUNT) {
        const unsigned char mask = kernaux_asm_i386_inportb(MASTER_DATA);
        kernaux_asm_i386_outportb(MASTER_DATA, mask | (1 << line));
    }
    else {
        const unsigned char mask = kernaux_asm_i386_inportb(SLAVE_DATA);
        kernaux_asm_i386_outportb(SLAVE_DATA, mask | (1 << (line - IRQS_COUNT)));
    }
}


void pic_remap(const unsigned char new_master_irq_start, const unsigned char new_slave_irq_start)
{
    kernaux_console_print("[INFO] pic: Remap the IRQ table.\n");

    // Remember IRQ numbers
    master_irq_start = new_master_irq_start;
    slave_irq_start  = new_slave_irq_start;

    // Save masks
    unsigned char master_mask = kernaux_asm_i386_inportb(MASTER_DATA);
    unsigned char slave_mask  = kernaux_asm_i386_inportb(SLAVE_DATA);

    // Start the initialization sequence
    kernaux_asm_i386_outportb(MASTER_COMMAND, 0x11);
    kernaux_asm_i386_outportb(SLAVE_COMMAND,  0x11);

    // Set IRQ vectors
    kernaux_asm_i386_outportb(MASTER_DATA, new_master_irq_start);
    kernaux_asm_i386_outportb(SLAVE_DATA,  new_slave_irq_start);

    // Connect master and slave with each other
    kernaux_asm_i386_outportb(MASTER_DATA, 0x04);
    kernaux_asm_i386_outportb(SLAVE_DATA,  0x02);

    // 8086/88 (MCS-80/85) mode
    kernaux_asm_i386_outportb(MASTER_DATA, 0x01);
    kernaux_asm_i386_outportb(SLAVE_DATA,  0x01);

    // Restore masks
    kernaux_asm_i386_outportb(MASTER_DATA, master_mask);
    kernaux_asm_i386_outportb(SLAVE_DATA,  slave_mask);
}

unsigned char pic_end_of_interrupt(const unsigned char irq)
{
    const unsigned char to_master = master_irq_start <= irq && irq < master_irq_start + IRQS_COUNT;
    const unsigned char to_slave  = slave_irq_start  <= irq && irq < slave_irq_start  + IRQS_COUNT;

    if (!(to_master || to_slave)) {
        return 0;
    }

    if (to_slave) {
        kernaux_asm_i386_outportb(SLAVE_COMMAND, 0x20);
    }

    kernaux_asm_i386_outportb(MASTER_COMMAND, 0x20);

    return 1;
}
