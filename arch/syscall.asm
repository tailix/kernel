#include "config.h"

[EXTERN syscall_handler]

[GLOBAL syscall_gate]

syscall_gate:
    cli

    pusha ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax

    mov ax, ds ; Lower 16-bits of eax = ds.
    push eax ; save the data segment descriptor

    mov ax, GDT_KERNEL_DS_SELECTOR
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call syscall_handler

    pop eax ; reload the original data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa                     ; Pops edi,esi,ebp...

    sti
    iret ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP
