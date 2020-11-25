void _start()
{
    asm(
        "mov $0, %eax \n\t"
        "mov $0, %ebx \n\t"
        "int $0x80    \n\t"
    );
}
