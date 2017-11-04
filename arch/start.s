.set MULTIBOOT_MAGIC, 0xE85250D6
.set MULTIBOOT_ARCH, 0
.set MULTIBOOT_LENGTH, 16 + 8
.set MULTIBOOT_CHECKSUM, -(MULTIBOOT_MAGIC + MULTIBOOT_ARCH + MULTIBOOT_LENGTH)

.section .multiboot
.align 4
.long MULTIBOOT_MAGIC
.long MULTIBOOT_ARCH
.long MULTIBOOT_LENGTH
.long MULTIBOOT_CHECKSUM
.long 0
.long 8

.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

.section .text

.global _start
.type _start, @function
_start:
    mov $stack_top, %esp // Initialize stack

    push %ebx // Multiboot information pointer
    push %eax // Multiboot magic number
    call init

    mov $stack_top, %esp // Initialize stack

    call main

    cli
1:
    hlt
    jmp 1b

.size _start, . - _start
