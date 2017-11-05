[BITS 32]

main:
    mov eax, 456
    int 0x80

    mov eax, 0 ; exit
    mov ebx, 0 ; error code
    int 0x80

    ret
