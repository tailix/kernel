#include "../config.h"

%macro INTERRUPT_NOERRCODE 2
[GLOBAL interrupt_%2]
interrupt_%2:
    cli
    push dword 0
    push dword %2
    jmp %1_wrapper
%endmacro

%macro INTERRUPT_ERRCODE 2
[GLOBAL interrupt_%2]
interrupt_%2:
    cli
    push dword %2
    jmp %1_wrapper
%endmacro

%macro INTERRUPT_COMMON 1
[EXTERN %1_handler]
%1_wrapper:
    pushad

    mov ax, ds ; Lower 16-bits of eax = ds.
    push dword eax ; save the data segment descriptor

    mov ax, GDT_KERNEL_DS_SELECTOR
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call %1_handler

    pop dword eax ; reload the original data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popad

    add esp, 8 ; Cleans up the pushed error code and pushed ISR number
    sti
    iret ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP
%endmacro

INTERRUPT_COMMON exception
INTERRUPT_COMMON hwint
INTERRUPT_COMMON syscall

; Protected mode exteptions

INTERRUPT_NOERRCODE exception, 0  ; #DE - Divide Error Exception
INTERRUPT_NOERRCODE exception, 1  ; #DB - Debug Exception
INTERRUPT_NOERRCODE exception, 2  ; NMI - Non-maskable interrupt
INTERRUPT_NOERRCODE exception, 3  ; #BP - Breakpoint Exception
INTERRUPT_NOERRCODE exception, 4  ; #OF - Overflow Exception
INTERRUPT_NOERRCODE exception, 5  ; #BR - BOUND Range Exceeded Exception
INTERRUPT_NOERRCODE exception, 6  ; #UD - Invalid Opcode Exception
INTERRUPT_NOERRCODE exception, 7  ; #NM - Device Not Available Exception
INTERRUPT_ERRCODE   exception, 8  ; #DF - Double Fault Exception
INTERRUPT_NOERRCODE exception, 9  ; Reserved - Coprocessor Segment Overrun
INTERRUPT_ERRCODE   exception, 10 ; #TS - Invalid TSS Exception
INTERRUPT_ERRCODE   exception, 11 ; #NP - Segment Not Present
INTERRUPT_ERRCODE   exception, 12 ; #SS - Stack Fault Exception
INTERRUPT_ERRCODE   exception, 13 ; #GP - General Protection Exception
INTERRUPT_ERRCODE   exception, 14 ; #PF - Page-Fault Exception
INTERRUPT_NOERRCODE exception, 15 ; Reserved
INTERRUPT_NOERRCODE exception, 16 ; #MF - x87 FPU Floating-Point Error
INTERRUPT_ERRCODE   exception, 17 ; #AC - Alignment Check Exception
INTERRUPT_NOERRCODE exception, 18 ; #MC - Machine-Check Exception
INTERRUPT_NOERRCODE exception, 19 ; #XF - SIMD Floating-Point Exception
INTERRUPT_NOERRCODE exception, 20 ; Reserved
INTERRUPT_NOERRCODE exception, 21 ; Reserved
INTERRUPT_NOERRCODE exception, 22 ; Reserved
INTERRUPT_NOERRCODE exception, 23 ; Reserved
INTERRUPT_NOERRCODE exception, 24 ; Reserved
INTERRUPT_NOERRCODE exception, 25 ; Reserved
INTERRUPT_NOERRCODE exception, 26 ; Reserved
INTERRUPT_NOERRCODE exception, 27 ; Reserved
INTERRUPT_NOERRCODE exception, 28 ; Reserved
INTERRUPT_NOERRCODE exception, 29 ; Reserved
INTERRUPT_NOERRCODE exception, 30 ; Reserved
INTERRUPT_NOERRCODE exception, 31 ; Reserved

; Hardware IRQs

INTERRUPT_NOERRCODE hwint, 32 ; Programmable Interval Timer
INTERRUPT_NOERRCODE hwint, 33 ; Keyboard
INTERRUPT_NOERRCODE hwint, 34 ; Slave PIC
INTERRUPT_NOERRCODE hwint, 35 ; COM 2/4
INTERRUPT_NOERRCODE hwint, 36 ; COM 1/3
INTERRUPT_NOERRCODE hwint, 37 ; LPT 2
INTERRUPT_NOERRCODE hwint, 38 ; Floppy Drive Controller
INTERRUPT_NOERRCODE hwint, 39 ; LPT 1

INTERRUPT_NOERRCODE hwint, 40 ; Real Time Clock
INTERRUPT_NOERRCODE hwint, 41 ; Master PIC
INTERRUPT_NOERRCODE hwint, 42 ; Reserved
INTERRUPT_NOERRCODE hwint, 43 ; Reserved
INTERRUPT_NOERRCODE hwint, 44 ; Reserved
INTERRUPT_NOERRCODE hwint, 45 ; Coprocessor exception
INTERRUPT_NOERRCODE hwint, 46 ; Hard Drive Controller
INTERRUPT_NOERRCODE hwint, 47 ; Reserved

; Syscalls

INTERRUPT_NOERRCODE syscall, INT_SYSCALL
