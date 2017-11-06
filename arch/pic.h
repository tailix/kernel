#ifndef KERNELMQ_INCLUDED_PIC
#define KERNELMQ_INCLUDED_PIC 1

void pic_remap(unsigned char master_irq_start, unsigned char slave_irq_start);

unsigned char pic_end_of_interrupt(unsigned char irq);

#endif
