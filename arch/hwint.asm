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

HWINT 0,  32
HWINT 1,  33
HWINT 2,  34
HWINT 3,  35
HWINT 4,  36
HWINT 5,  37
HWINT 6,  38
HWINT 7,  39
HWINT 8,  40
HWINT 9,  41
HWINT 10, 42
HWINT 11, 43
HWINT 12, 44
HWINT 13, 45
HWINT 14, 46
HWINT 15, 47

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
