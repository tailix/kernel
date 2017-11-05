#include "config.h"

%include "interrupt.asm"

[EXTERN exception_handler]

INTERRUPT_NOERRCODE 0  ; #DE - Divide Error Exception
INTERRUPT_NOERRCODE 1  ; #DB - Debug Exception
INTERRUPT_NOERRCODE 2  ; NMI - Non-maskable interrupt
INTERRUPT_NOERRCODE 3  ; #BP - Breakpoint Exception
INTERRUPT_NOERRCODE 4  ; #OF - Overflow Exception
INTERRUPT_NOERRCODE 5  ; #BR - BOUND Range Exceeded Exception
INTERRUPT_NOERRCODE 6  ; #UD - Invalid Opcode Exception
INTERRUPT_NOERRCODE 7  ; #NM - Device Not Available Exception
INTERRUPT_ERRCODE   8  ; #DF - Double Fault Exception
INTERRUPT_NOERRCODE 9  ; Reserved - Coprocessor Segment Overrun
INTERRUPT_ERRCODE   10 ; #TS - Invalid TSS Exception
INTERRUPT_ERRCODE   11 ; #NP - Segment Not Present
INTERRUPT_ERRCODE   12 ; #SS - Stack Fault Exception
INTERRUPT_ERRCODE   13 ; #GP - General Protection Exception
INTERRUPT_ERRCODE   14 ; #PF - Page-Fault Exception
INTERRUPT_NOERRCODE 15 ; Reserved
INTERRUPT_NOERRCODE 16 ; #MF - x87 FPU Floating-Point Error
INTERRUPT_ERRCODE   17 ; #AC - Alignment Check Exception
INTERRUPT_NOERRCODE 18 ; #MC - Machine-Check Exception
INTERRUPT_NOERRCODE 19 ; #XF - SIMD Floating-Point Exception
INTERRUPT_NOERRCODE 20 ; Reserved
INTERRUPT_NOERRCODE 21 ; Reserved
INTERRUPT_NOERRCODE 22 ; Reserved
INTERRUPT_NOERRCODE 23 ; Reserved
INTERRUPT_NOERRCODE 24 ; Reserved
INTERRUPT_NOERRCODE 25 ; Reserved
INTERRUPT_NOERRCODE 26 ; Reserved
INTERRUPT_NOERRCODE 27 ; Reserved
INTERRUPT_NOERRCODE 28 ; Reserved
INTERRUPT_NOERRCODE 29 ; Reserved
INTERRUPT_NOERRCODE 30 ; Reserved
INTERRUPT_NOERRCODE 31 ; Reserved

INTERRUPT_COMMON exception_handler, GDT_KERNEL_DS_SELECTOR
