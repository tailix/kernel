#include "config.h"

%include "interrupt.asm"

INTERRUPT_COMMON syscall_handler, GDT_KERNEL_DS_SELECTOR

INTERRUPT_NOERRCODE INT_SYSCALL
