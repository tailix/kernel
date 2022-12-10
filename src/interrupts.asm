#include "config.h"

[EXTERN interrupts_handler]

%macro NOERRCODE 1
[GLOBAL interrupts_cb_%1]
interrupts_cb_%1:
    push dword 0
    push dword %1
    jmp common_part
%endmacro

%macro ERRCODE 1
[GLOBAL interrupts_cb_%1]
interrupts_cb_%1:
    push dword %1
    jmp common_part
%endmacro

common_part:
    pushad

    mov ax, ds ; Lower 16-bits of eax = ds.
    push dword eax ; save the data segment descriptor

    mov ax, GDT_KERNEL_DS_SELECTOR
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call interrupts_handler

    pop dword eax ; reload the original data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popad

    add esp, 8 ; Cleans up the pushed error code and pushed ISR number
    iret ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP

; Protected mode exteptions

NOERRCODE 0x00 ; #DE - Divide Error Exception
NOERRCODE 0x01 ; #DB - Debug Exception
NOERRCODE 0x02 ; NMI - Non-maskable interrupt
NOERRCODE 0x03 ; #BP - Breakpoint Exception
NOERRCODE 0x04 ; #OF - Overflow Exception
NOERRCODE 0x05 ; #BR - BOUND Range Exceeded Exception
NOERRCODE 0x06 ; #UD - Invalid Opcode Exception
NOERRCODE 0x07 ; #NM - Device Not Available Exception
ERRCODE   0x08 ; #DF - Double Fault Exception
NOERRCODE 0x09 ; Reserved - Coprocessor Segment Overrun
ERRCODE   0x0a ; #TS - Invalid TSS Exception
ERRCODE   0x0b ; #NP - Segment Not Present
ERRCODE   0x0c ; #SS - Stack Fault Exception
ERRCODE   0x0d ; #GP - General Protection Exception
ERRCODE   0x0e ; #PF - Page-Fault Exception
NOERRCODE 0x0f ; Reserved
NOERRCODE 0x10 ; #MF - x87 FPU Floating-Point Error
ERRCODE   0x11 ; #AC - Alignment Check Exception
NOERRCODE 0x12 ; #MC - Machine-Check Exception
NOERRCODE 0x13 ; #XF - SIMD Floating-Point Exception
NOERRCODE 0x14 ; Reserved
NOERRCODE 0x15 ; Reserved
NOERRCODE 0x16 ; Reserved
NOERRCODE 0x17 ; Reserved
NOERRCODE 0x18 ; Reserved
NOERRCODE 0x19 ; Reserved
NOERRCODE 0x1a ; Reserved
NOERRCODE 0x1b ; Reserved
NOERRCODE 0x1c ; Reserved
NOERRCODE 0x1d ; Reserved
NOERRCODE 0x1e ; Reserved
NOERRCODE 0x1f ; Reserved

; Hardware IRQs

NOERRCODE 0x20 ; Programmable Interval Timer
NOERRCODE 0x21 ; Keyboard
NOERRCODE 0x22 ; Slave PIC
NOERRCODE 0x23 ; COM 2/4
NOERRCODE 0x24 ; COM 1/3
NOERRCODE 0x25 ; LPT 2
NOERRCODE 0x26 ; Floppy Drive Controller
NOERRCODE 0x27 ; LPT 1

NOERRCODE 0x28 ; Real Time Clock
NOERRCODE 0x29 ; Master PIC
NOERRCODE 0x2a ; Reserved
NOERRCODE 0x2b ; Reserved
NOERRCODE 0x2c ; Reserved
NOERRCODE 0x2d ; Coprocessor exception
NOERRCODE 0x2e ; Hard Drive Controller
NOERRCODE 0x2f ; Reserved
