[GLOBAL read_cr0]
[GLOBAL read_cr4]

[GLOBAL write_cr0]
[GLOBAL write_cr3]
[GLOBAL write_cr4]

read_cr0:
    mov eax, cr0
    ret

read_cr4:
    mov eax, cr4
    ret

write_cr0:
    mov eax, [esp+4]
    mov cr0, eax
    ret

write_cr3:
    mov eax, [esp+4]
    mov cr3, eax
    ret

write_cr4:
    mov eax, [esp+4]
    mov cr4, eax
    ret
