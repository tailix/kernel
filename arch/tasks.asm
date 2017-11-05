#include "config.h"

[GLOBAL tasks_switch_to_user]

tasks_switch_to_user:
    mov ebx, [esp+4]

    mov ax, GDT_USER_DS_SELECTOR
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov eax, esp
    push GDT_USER_DS_SELECTOR
    push eax
    pushf
    push GDT_USER_CS_SELECTOR
    push ebx
    iret
