#include "config.h"

[EXTERN exception_handler]

%macro EXCEPTION_NOERRCODE 1
[GLOBAL exception_%1]
exception_%1:
    cli
    push byte 0
    push byte %1
    jmp exception_common_stub
%endmacro

%macro EXCEPTION_ERRCODE 1
[GLOBAL exception_%1]
exception_%1:
    cli
    push byte %1
    jmp exception_common_stub
%endmacro

EXCEPTION_NOERRCODE 0  ; #DE - Divide Error Exception
EXCEPTION_NOERRCODE 1  ; #DB - Debug Exception
EXCEPTION_NOERRCODE 2  ; NMI - Non-maskable interrupt
EXCEPTION_NOERRCODE 3  ; #BP - Breakpoint Exception
EXCEPTION_NOERRCODE 4  ; #OF - Overflow Exception
EXCEPTION_NOERRCODE 5  ; #BR - BOUND Range Exceeded Exception
EXCEPTION_NOERRCODE 6  ; #UD - Invalid Opcode Exception
EXCEPTION_NOERRCODE 7  ; #NM - Device Not Available Exception
EXCEPTION_ERRCODE   8  ; #DF - Double Fault Exception
EXCEPTION_NOERRCODE 9  ; Reserved - Coprocessor Segment Overrun
EXCEPTION_ERRCODE   10 ; #TS - Invalid TSS Exception
EXCEPTION_ERRCODE   11 ; #NP - Segment Not Present
EXCEPTION_ERRCODE   12 ; #SS - Stack Fault Exception
EXCEPTION_ERRCODE   13 ; #GP - General Protection Exception
EXCEPTION_ERRCODE   14 ; #PF - Page-Fault Exception
EXCEPTION_NOERRCODE 15 ; Reserved
EXCEPTION_NOERRCODE 16 ; #MF - x87 FPU Floating-Point Error
EXCEPTION_ERRCODE   17 ; #AC - Alignment Check Exception
EXCEPTION_NOERRCODE 18 ; #MC - Machine-Check Exception
EXCEPTION_NOERRCODE 19 ; #XF - SIMD Floating-Point Exception
EXCEPTION_NOERRCODE 20 ; Reserved
EXCEPTION_NOERRCODE 21 ; Reserved
EXCEPTION_NOERRCODE 22 ; Reserved
EXCEPTION_NOERRCODE 23 ; Reserved
EXCEPTION_NOERRCODE 24 ; Reserved
EXCEPTION_NOERRCODE 25 ; Reserved
EXCEPTION_NOERRCODE 26 ; Reserved
EXCEPTION_NOERRCODE 27 ; Reserved
EXCEPTION_NOERRCODE 28 ; Reserved
EXCEPTION_NOERRCODE 29 ; Reserved
EXCEPTION_NOERRCODE 30 ; Reserved
EXCEPTION_NOERRCODE 31 ; Reserved

exception_common_stub:
    pusha ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax

    mov ax, ds ; Lower 16-bits of eax = ds.
    push eax ; save the data segment descriptor

    mov ax, GDT_KERNEL_DS_SELECTOR
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call exception_handler

    pop eax ; reload the original data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa                     ; Pops edi,esi,ebp...
    add esp, 8 ; Cleans up the pushed error code and pushed ISR number
    sti
    iret ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP
