#include "pic.h"

#include "logger.h"
#include "asm.h"

#define PIC_MASTER_COMMAND 0x20
#define PIC_MASTER_DATA    0x21

#define PIC_SLAVE_COMMAND 0xA0
#define PIC_SLAVE_DATA    0xA1

void pic_remap(const unsigned char master_irq_start, const unsigned char slave_irq_start)
{
    logger_info_from("pic", "Remap the IRQ table.");

    // Start the initialization sequence
    outportb(PIC_MASTER_COMMAND, 0x11);
    outportb(PIC_SLAVE_COMMAND,  0x11);

    // Set IRQ vectors
    outportb(PIC_MASTER_DATA, master_irq_start);
    outportb(PIC_SLAVE_DATA,  slave_irq_start);

    // Connect master and slave with each other
    outportb(PIC_MASTER_DATA, 0x04);
    outportb(PIC_SLAVE_DATA,  0x02);

    // 8086/88 (MCS-80/85) mode
    outportb(PIC_MASTER_DATA, 0x01);
    outportb(PIC_SLAVE_DATA,  0x01);

    outportb(PIC_MASTER_DATA, 0x00);
    outportb(PIC_SLAVE_DATA,  0x00);
}
