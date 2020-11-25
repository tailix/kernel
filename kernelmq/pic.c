#include "pic.h"

#include "logger.h"
#include "asm.h"

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

    outportb(MASTER_DATA, 0);
    outportb(SLAVE_DATA,  0);
}

void pic_disable_all()
{
    logger_info_from("pic", "Disable all IRQs.");

    outportb(MASTER_DATA, 0xFF);
    outportb(SLAVE_DATA,  0xFF);
}

void pic_enable(const unsigned char line)
{
    if (line >= IRQS_TOTAL) {
        logger_warn_from("pic", "Invalid line %u.", line);
        return;
    }

    logger_info_from("pic", "Enable line %u.", line);

    if (line < IRQS_COUNT) {
        const unsigned char mask = inportb(MASTER_DATA);
        outportb(MASTER_DATA, mask & ~(1 << line));
    }
    else {
        const unsigned char mask = inportb(SLAVE_DATA);
        outportb(SLAVE_DATA, mask & ~(1 << (line - IRQS_COUNT)));
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
        const unsigned char mask = inportb(MASTER_DATA);
        outportb(MASTER_DATA, mask | (1 << line));
    }
    else {
        const unsigned char mask = inportb(SLAVE_DATA);
        outportb(SLAVE_DATA, mask | (1 << (line - IRQS_COUNT)));
    }
}


void pic_remap(const unsigned char new_master_irq_start, const unsigned char new_slave_irq_start)
{
    logger_info_from("pic", "Remap the IRQ table.");

    // Remember IRQ numbers
    master_irq_start = new_master_irq_start;
    slave_irq_start  = new_slave_irq_start;

    // Save masks
    unsigned char master_mask = inportb(MASTER_DATA);
    unsigned char slave_mask  = inportb(SLAVE_DATA);

    // Start the initialization sequence
    outportb(MASTER_COMMAND, 0x11);
    outportb(SLAVE_COMMAND,  0x11);

    // Set IRQ vectors
    outportb(MASTER_DATA, new_master_irq_start);
    outportb(SLAVE_DATA,  new_slave_irq_start);

    // Connect master and slave with each other
    outportb(MASTER_DATA, 0x04);
    outportb(SLAVE_DATA,  0x02);

    // 8086/88 (MCS-80/85) mode
    outportb(MASTER_DATA, 0x01);
    outportb(SLAVE_DATA,  0x01);

    // Restore masks
    outportb(MASTER_DATA, master_mask);
    outportb(SLAVE_DATA,  slave_mask);
}

unsigned char pic_end_of_interrupt(const unsigned char irq)
{
    const unsigned char to_master = master_irq_start <= irq && irq < master_irq_start + IRQS_COUNT;
    const unsigned char to_slave  = slave_irq_start  <= irq && irq < slave_irq_start  + IRQS_COUNT;

    if (!(to_master || to_slave)) {
        return 0;
    }

    if (to_slave) {
        outportb(SLAVE_COMMAND, 0x20);
    }

    outportb(MASTER_COMMAND, 0x20);

    return 1;
}