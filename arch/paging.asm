[GLOBAL write_cr3]

write_cr3:
    mov eax, [esp+4]
    mov cr3, eax
    ret
