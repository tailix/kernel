%macro INTERRUPT_NOERRCODE 1
[GLOBAL interrupt_%1]
interrupt_%1:
    cli
    push byte 0
    push byte %1
    jmp interrupt_common
%endmacro

%macro INTERRUPT_ERRCODE 1
[GLOBAL interrupt_%1]
interrupt_%1:
    cli
    push byte %1
    jmp interrupt_common
%endmacro

%macro INTERRUPT_COMMON 2
interrupt_common:
    pusha ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax

    mov ax, ds ; Lower 16-bits of eax = ds.
    push eax ; save the data segment descriptor

    mov ax, %2
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call %1

    pop eax ; reload the original data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa                     ; Pops edi,esi,ebp...
    add esp, 8 ; Cleans up the pushed error code and pushed ISR number
    sti
    iret ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP
%endmacro
