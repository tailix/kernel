#include "config.h"

%include "interrupt.asm"

[EXTERN syscall_handler]

INTERRUPT_NOERRCODE INT_SYSCALL

INTERRUPT_COMMON syscall_handler, GDT_KERNEL_DS_SELECTOR
