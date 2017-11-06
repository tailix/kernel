#include "pic.h"

#include "logger.h"
#include "asm.h"

#define PIC_MASTER_COMMAND 0x20
#define PIC_MASTER_DATA    0x21

#define PIC_SLAVE_COMMAND 0xA0
#define PIC_SLAVE_DATA    0xA1

void pic_initialize()
{
    logger_info_from("pic", "Remap the IRQ table.");

    outportb(PIC_MASTER_COMMAND, 0x11);
    outportb(PIC_SLAVE_COMMAND,  0x11);

    outportb(PIC_MASTER_DATA, 0x20);
    outportb(PIC_SLAVE_DATA,  0x28);

    outportb(PIC_MASTER_DATA, 0x04);
    outportb(PIC_SLAVE_DATA,  0x02);

    outportb(PIC_MASTER_DATA, 0x01);
    outportb(PIC_SLAVE_DATA,  0x01);

    outportb(PIC_MASTER_DATA, 0x00);
    outportb(PIC_SLAVE_DATA,  0x00);
}
