#ifndef KERNELMQ_INCLUDED_PIC
#define KERNELMQ_INCLUDED_PIC 1

void pic_remap(unsigned char master_irq_start, unsigned char slave_irq_start);

void pic_enable_all();
void pic_disable_all();

unsigned char pic_end_of_interrupt(unsigned char irq);

#endif
