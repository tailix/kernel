#include "config.h"

[GLOBAL gdt_flush]

gdt_flush:
    mov eax, [esp+4]
    lgdt [eax]

    mov ax, GDT_KERNEL_DS_SELECTOR
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp GDT_KERNEL_CS_SELECTOR:.flush
.flush:
    ret
