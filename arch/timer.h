#ifndef KERNELMQ_INCLUDED_TIMER
#define KERNELMQ_INCLUDED_TIMER 1

#include "hwint.h"

typedef hwint_handler_t timer_handler_t;

void timer_initialize(unsigned int frequency);

void timer_register_handler(timer_handler_t handler);
void timer_unregister_handler();

#endif
