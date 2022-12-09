#ifndef KERNEL_INCLUDED_INTERRUPTS
#define KERNEL_INCLUDED_INTERRUPTS

#include <stdint.h>

void interrupts_init();
void interrupts_load();
void interrupts_setup(uint16_t kernel_cs_selector);

#endif
