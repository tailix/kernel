CCPREFIX = i686-elf-

AS = $(CCPREFIX)as
CC = $(CCPREFIX)gcc

OBJS = boot.s.o main.c.o logger.c.o console.c.o gdt.c.o gdt.asm.o idt.c.o idt.asm.o isr.c.o isr.asm.o multiboot.c.o

all: kernel

clean:
	rm -f kernel $(OBJS)

kernel: $(OBJS)
	$(CC) -T linker.ld -o $@ -ffreestanding -nostdlib -lgcc $(OBJS)
	grub-file --is-x86-multiboot2 $@

%.c.o: %.c
	$(CC) -c $< -o $@ -std=gnu99 -ffreestanding -nostdinc -fno-builtin -fno-stack-protector -Wall -Wextra -I "$(I)"

%.s.o: %.s
	$(AS) $< -o $@

%.asm.o: %.asm
	nasm -felf -o $@ $<
