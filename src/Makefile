CCPREFIX = i686-elf-

AS = $(CCPREFIX)as
CC = $(CCPREFIX)gcc

OBJS = boot.s.o main.c.o logger.c.o console.c.o gdt.c.o gdt.asm.o idt.c.o idt.asm.o isr.c.o isr.asm.o

all: kernel

clean:
	rm -f kernel $(OBJS)

kernel: $(OBJS)
	$(CC) -T linker.ld -o $@ -ffreestanding -nostdlib -O2 -lgcc $(OBJS)

%.c.o: %.c
	$(CC) -c $< -o $@ -std=gnu99 -ffreestanding -O2 -Wall -Wextra

%.s.o: %.s
	$(AS) $< -o $@

%.asm.o: %.asm
	nasm -felf -o $@ $<
