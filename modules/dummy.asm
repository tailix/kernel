main:
    mov eax, 512
    int 0x80

    mov eax, 35000
    int 0x80

    mov eax, 0 ; exit
    mov ebx, 0 ; error code
    int 0x80

    ret
