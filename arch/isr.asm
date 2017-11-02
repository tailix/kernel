#include "config.h"

[EXTERN isr]

%macro ISR_NOERRCODE 1
[GLOBAL isr%1]
isr%1:
    cli
    push byte 0
    push byte %1
    jmp isr_common_stub
%endmacro

%macro ISR_ERRCODE 1
[GLOBAL isr%1]
isr%1:
    cli
    push byte %1
    jmp isr_common_stub
%endmacro

ISR_NOERRCODE 0  ; #DE - Divide Error Exception
ISR_NOERRCODE 1  ; #DB - Debug Exception
ISR_NOERRCODE 2  ; NMI - Non-maskable interrupt
ISR_NOERRCODE 3  ; #BP - Breakpoint Exception
ISR_NOERRCODE 4  ; #OF - Overflow Exception
ISR_NOERRCODE 5  ; #BR - BOUND Range Exceeded Exception
ISR_NOERRCODE 6  ; #UD - Invalid Opcode Exception
ISR_NOERRCODE 7  ; #NM - Device Not Available Exception
ISR_ERRCODE   8  ; #DF - Double Fault Exception
ISR_NOERRCODE 9  ; Reserved - Coprocessor Segment Overrun
ISR_ERRCODE   10 ; #TS - Invalid TSS Exception
ISR_ERRCODE   11 ; #NP - Segment Not Present
ISR_ERRCODE   12 ; #SS - Stack Fault Exception
ISR_ERRCODE   13 ; #GP - General Protection Exception
ISR_ERRCODE   14 ; #PF - Page-Fault Exception
ISR_NOERRCODE 15 ; Reserved
ISR_NOERRCODE 16 ; #MF - x87 FPU Floating-Point Error
ISR_ERRCODE   17 ; #AC - Alignment Check Exception
ISR_NOERRCODE 18 ; #MC - Machine-Check Exception
ISR_NOERRCODE 19 ; #XF - SIMD Floating-Point Exception
ISR_NOERRCODE 20 ; Reserved
ISR_NOERRCODE 21 ; Reserved
ISR_NOERRCODE 22 ; Reserved
ISR_NOERRCODE 23 ; Reserved
ISR_NOERRCODE 24 ; Reserved
ISR_NOERRCODE 25 ; Reserved
ISR_NOERRCODE 26 ; Reserved
ISR_NOERRCODE 27 ; Reserved
ISR_NOERRCODE 28 ; Reserved
ISR_NOERRCODE 29 ; Reserved
ISR_NOERRCODE 30 ; Reserved
ISR_NOERRCODE 31 ; Reserved

isr_common_stub:
   pusha ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax

   mov ax, ds ; Lower 16-bits of eax = ds.
   push eax ; save the data segment descriptor

   mov ax, GDT_KERNEL_DS_SELECTOR
   mov ds, ax
   mov es, ax
   mov fs, ax
   mov gs, ax

   call isr

   pop eax ; reload the original data segment descriptor
   mov ds, ax
   mov es, ax
   mov fs, ax
   mov gs, ax

   popa                     ; Pops edi,esi,ebp...
   add esp, 8 ; Cleans up the pushed error code and pushed ISR number
   sti
   iret ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP
