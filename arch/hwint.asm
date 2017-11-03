#include "config.h"

[EXTERN hwint_handler]

%macro HWINT 2
[GLOBAL hwint_%1]
hwint_%1:
    cli
    push byte 0
    push byte %2
    jmp hwint_common_stub
%endmacro

HWINT 0,  32 ; Programmable Interval Timer
HWINT 1,  33 ; Keyboard
HWINT 2,  34 ; Slave PIC
HWINT 3,  35 ; COM 2/4
HWINT 4,  36 ; COM 1/3
HWINT 5,  37 ; LPT 2
HWINT 6,  38 ; Floppy Drive Controller
HWINT 7,  39 ; LPT 1

HWINT 8,  40 ; Real Time Clock
HWINT 9,  41 ; Master PIC
HWINT 10, 42 ; Reserved
HWINT 11, 43 ; Reserved
HWINT 12, 44 ; Reserved
HWINT 13, 45 ; Coprocessor exception
HWINT 14, 46 ; Hard Drive Controller
HWINT 15, 47 ; Reserved

hwint_common_stub:
    pusha ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax

    mov ax, ds ; Lower 16-bits of eax = ds.
    push eax ; save the data segment descriptor

    mov ax, GDT_KERNEL_DS_SELECTOR
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call hwint_handler

    pop eax ; reload the original data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa                     ; Pops edi,esi,ebp...
    add esp, 8 ; Cleans up the pushed error code and pushed ISR number
    sti
    iret ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP
