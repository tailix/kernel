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
.skip 16384 # 16 KiB
_kernel_stack_top:

.global _kernel_stack_top

.section .text

.global _start

.type _start, @function
.type halt,   @function

_start:
    mov $_kernel_stack_top, %esp // Initialize stack

    push %ebx // Multiboot information pointer
    push %eax // Multiboot magic number
    call init

    mov $_kernel_stack_top, %esp // Initialize stack

    push %eax // Kernel information pointer
    call main

    call halt

.size _start, . - _start
