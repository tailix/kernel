#include "pic.h"

#include "logger.h"
#include "asm.h"

#define MASTER_COMMAND 0x20
#define MASTER_DATA    0x21

#define SLAVE_COMMAND 0xA0
#define SLAVE_DATA    0xA1

void pic_remap(const unsigned char master_irq_start, const unsigned char slave_irq_start)
{
    logger_info_from("pic", "Remap the IRQ table.");

    // Save masks
    unsigned char master_mask = inportb(MASTER_DATA);
    unsigned char slave_mask  = inportb(SLAVE_DATA);

    // Start the initialization sequence
    outportb(MASTER_COMMAND, 0x11);
    outportb(SLAVE_COMMAND,  0x11);

    // Set IRQ vectors
    outportb(MASTER_DATA, master_irq_start);
    outportb(SLAVE_DATA,  slave_irq_start);

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
