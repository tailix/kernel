#include "pic.h"

#include "logger.h"

#include <kernaux/arch/x86.h>

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
    logger_info_from("pic", "Enable all IRQs.");

    kernaux_arch_x86_outportb(MASTER_DATA, 0);
    kernaux_arch_x86_outportb(SLAVE_DATA,  0);
}

void pic_disable_all()
{
    logger_info_from("pic", "Disable all IRQs.");

    kernaux_arch_x86_outportb(MASTER_DATA, 0xFF);
    kernaux_arch_x86_outportb(SLAVE_DATA,  0xFF);
}

void pic_enable(const unsigned char line)
{
    if (line >= IRQS_TOTAL) {
        logger_warn_from("pic", "Invalid line %u.", line);
        return;
    }

    logger_info_from("pic", "Enable line %u.", line);

    if (line < IRQS_COUNT) {
        const unsigned char mask = kernaux_arch_x86_inportb(MASTER_DATA);
        kernaux_arch_x86_outportb(MASTER_DATA, mask & ~(1 << line));
    }
    else {
        const unsigned char mask = kernaux_arch_x86_inportb(SLAVE_DATA);
        kernaux_arch_x86_outportb(SLAVE_DATA, mask & ~(1 << (line - IRQS_COUNT)));
    }
}

void pic_disable(const unsigned char line)
{
    if (line >= IRQS_TOTAL) {
        logger_warn_from("pic", "Invalid line %u.", line);
        return;
    }

    logger_info_from("pic", "Disable line %u.", line);

    if (line < IRQS_COUNT) {
        const unsigned char mask = kernaux_arch_x86_inportb(MASTER_DATA);
        kernaux_arch_x86_outportb(MASTER_DATA, mask | (1 << line));
    }
    else {
        const unsigned char mask = kernaux_arch_x86_inportb(SLAVE_DATA);
        kernaux_arch_x86_outportb(SLAVE_DATA, mask | (1 << (line - IRQS_COUNT)));
    }
}


void pic_remap(const unsigned char new_master_irq_start, const unsigned char new_slave_irq_start)
{
    logger_info_from("pic", "Remap the IRQ table.");

    // Remember IRQ numbers
    master_irq_start = new_master_irq_start;
    slave_irq_start  = new_slave_irq_start;

    // Save masks
    unsigned char master_mask = kernaux_arch_x86_inportb(MASTER_DATA);
    unsigned char slave_mask  = kernaux_arch_x86_inportb(SLAVE_DATA);

    // Start the initialization sequence
    kernaux_arch_x86_outportb(MASTER_COMMAND, 0x11);
    kernaux_arch_x86_outportb(SLAVE_COMMAND,  0x11);

    // Set IRQ vectors
    kernaux_arch_x86_outportb(MASTER_DATA, new_master_irq_start);
    kernaux_arch_x86_outportb(SLAVE_DATA,  new_slave_irq_start);

    // Connect master and slave with each other
    kernaux_arch_x86_outportb(MASTER_DATA, 0x04);
    kernaux_arch_x86_outportb(SLAVE_DATA,  0x02);

    // 8086/88 (MCS-80/85) mode
    kernaux_arch_x86_outportb(MASTER_DATA, 0x01);
    kernaux_arch_x86_outportb(SLAVE_DATA,  0x01);

    // Restore masks
    kernaux_arch_x86_outportb(MASTER_DATA, master_mask);
    kernaux_arch_x86_outportb(SLAVE_DATA,  slave_mask);
}

unsigned char pic_end_of_interrupt(const unsigned char irq)
{
    const unsigned char to_master = master_irq_start <= irq && irq < master_irq_start + IRQS_COUNT;
    const unsigned char to_slave  = slave_irq_start  <= irq && irq < slave_irq_start  + IRQS_COUNT;

    if (!(to_master || to_slave)) {
        return 0;
    }

    if (to_slave) {
        kernaux_arch_x86_outportb(SLAVE_COMMAND, 0x20);
    }

    kernaux_arch_x86_outportb(MASTER_COMMAND, 0x20);

    return 1;
}
