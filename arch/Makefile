AS = $(CCPREFIX)as
CC = $(CCPREFIX)gcc

OBJS = start.s.o
OBJS += init.c.o
OBJS += main.c.o

OBJS += logger.c.o
OBJS += console.c.o
OBJS += kprintf.c.o

OBJS += multiboot.c.o
OBJS += memory.c.o
OBJS += kmalloc.c.o
OBJS += paging.c.o

OBJS += protected.c.o protected.asm.cpp.o
OBJS += exception.c.o exception.asm.cpp.o
OBJS += hwint.c.o     hwint.asm.cpp.o

OBJS += timer.c.o

all: kernel

clean:
	rm -f kernel $(OBJS)

kernel: $(OBJS)
	$(CC) -T linker.ld -o $@ -ffreestanding -nostdlib -lgcc $(OBJS) $(LIBK)
	grub-file --is-x86-multiboot2 $@

%.c.o: %.c
	$(CC) -c $< -o $@ -std=gnu99 -ffreestanding -nostdinc -fno-builtin -fno-stack-protector -Wall -Wextra -I "$(I)"

%.s.o: %.s
	$(AS) $< -o $@

%.asm.cpp.o: %.asm.cpp
	nasm -felf -o $@ $<

%.asm.cpp: %.asm
	cpp -P $< $@
