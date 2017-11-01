.set MULTIBOOT_1_MAGIC, 0x1BADB002
.set MULTIBOOT_1_FLAGS, (1 << 0) | (1 << 1) // Align, meminfo
.set MULTIBOOT_1_CHECKSUM, -(MULTIBOOT_1_MAGIC + MULTIBOOT_1_FLAGS)

.set MULTIBOOT_2_MAGIC, 0xE85250D6
.set MULTIBOOT_2_ARCH, 0
.set MULTIBOOT_2_LENGTH, 16
.set MULTIBOOT_2_CHECKSUM, -(MULTIBOOT_2_MAGIC + MULTIBOOT_2_ARCH + MULTIBOOT_2_LENGTH)

.section .multiboot2
.align 4
.long MULTIBOOT_2_MAGIC
.long MULTIBOOT_2_ARCH
.long MULTIBOOT_2_LENGTH
.long MULTIBOOT_2_CHECKSUM

.section .multiboot1
.align 4
.long MULTIBOOT_1_MAGIC
.long MULTIBOOT_1_FLAGS
.long MULTIBOOT_1_CHECKSUM

.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

.section .text

.global _start
.type _start, @function
_start:
    mov $stack_top, %esp
    push %eax // uint32_t multiboot_magic
    call main
    cli
1:
    hlt
    jmp 1b

.size _start, . - _start
