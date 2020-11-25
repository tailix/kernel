void _start()
{
    asm(
        "mov $0,   %eax \n\t"
        "mov $0,   %ebx \n\t"
        "int $0x80      \n\t"

        "mov $0,   %eax \n\t"
        "mov $1,   %ebx \n\t"
        "int $0x80      \n\t"

        "mov $1,   %eax \n\t"
        "mov $0,   %ebx \n\t"
        "int $0x80      \n\t"

        "mov $2,   %eax \n\t"
        "mov $0,   %ebx \n\t"
        "int $0x80      \n\t"

        "mov $256, %eax \n\t"
        "mov $0,   %ebx \n\t"
        "int $0x80      \n\t"

        "mov $257, %eax \n\t"
        "mov $0,   %ebx \n\t"
        "int $0x80      \n\t"

        "mov $258, %eax \n\t"
        "mov $0,   %ebx \n\t"
        "int $0x80      \n\t"
    );
}
