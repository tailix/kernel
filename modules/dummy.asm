main:
    mov eax, 0
    int 0x80

    mov eax, 512
    int 0x80

    mov eax, 35000
    int 0x80

    ret
