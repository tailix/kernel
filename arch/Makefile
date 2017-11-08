AS = $(CCPREFIX)as
CC = $(CCPREFIX)gcc

# Architecture-dependent
OBJS = start.s.o
OBJS += init.c.o
OBJS += multiboot.c.o
OBJS += panic.c.o  panic.asm.cpp.o
OBJS += memory.c.o
OBJS += paging.c.o paging.asm.cpp.o

# Architecture-independent
OBJS += info.c.o
OBJS += main.c.o

# Built-in drivers
OBJS += console.c.o
OBJS += pic.c.o
OBJS += timer.c.o
OBJS += keyboard.c.o

# For debugging
OBJS += logger.c.o

OBJS += protected.c.o protected.asm.cpp.o

OBJS += tss.c.o tss.asm.cpp.o
OBJS += tasks.asm.cpp.o

OBJS += interrupt.asm.cpp.o

OBJS += exception.c.o
OBJS += hwint.c.o
OBJS += syscall.c.o

all: kernel

clean:
	rm -f kernel $(OBJS)

kernel: $(OBJS)
	$(CC) -T linker.ld -o $@ -ffreestanding -nostdlib -lgcc $(OBJS) $(LIBK)
	grub-file --is-x86-multiboot2 $@

%.c.o: %.c
	$(CC) -c $< -o $@ -std=gnu99 -ffreestanding -nostdinc -fno-builtin -fno-stack-protector -Wall -Wextra -I "$(INCLUDE)"

%.s.o: %.s
	$(AS) $< -o $@

%.asm.cpp.o: %.asm.cpp
	nasm -felf32 -o $@ $<

%.asm.cpp: %.asm
	cpp -P $< $@
