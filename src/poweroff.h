#ifndef KERNEL_INCLUDED_POWEROFF
#define KERNEL_INCLUDED_POWEROFF 1

#include <stdbool.h>

#define POWEROFF_INT 3
#define POWEROFF_ASM "int $3"

extern bool poweroff_doing;

void poweroff();

#endif
