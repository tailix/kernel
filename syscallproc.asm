main:
    xor eax, eax
    xor ebx, ebx
    int 0x80

    xor eax, eax
    mov ebx, 1
    int 0x80

    mov eax, 1
    xor ebx, ebx
    int 0x80

    mov eax, 2
    xor ebx, ebx
    int 0x80

    mov eax, 256
    xor ebx, ebx
    int 0x80

    mov eax, 257
    xor ebx, ebx
    int 0x80

    mov eax, 258
    xor ebx, ebx
    int 0x80
