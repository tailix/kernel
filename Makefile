CCPREFIX = i686-elf-

AR = $(CCPREFIX)ar
AS = $(CCPREFIX)as
CC = $(CCPREFIX)gcc

KERNEL = rootfs/boot/kernelmq.multiboot2

CFLAGS = -std=gnu99 -ffreestanding -nostdinc -fno-builtin -fno-stack-protector -Wall -Wextra

IMAGE = image.iso

# Architecture-dependent
OBJS = start.s.o
OBJS += main.c.o
OBJS += init.c.o
OBJS += multiboot.c.o
OBJS += panic.c.o  panic.asm.cpp.o
OBJS += pfa.c.o
OBJS += paging.c.o paging.asm.cpp.o
OBJS += pagedir.c.o

# Architecture-independent
OBJS += info.c.o
OBJS += memset.c.o
OBJS += strlen.c.o
OBJS += itoa.c.o
OBJS += strncpy.c.o

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

OBJS := $(addprefix src/, $(OBJS))

run: $(IMAGE)
	qemu-system-i386 -cdrom $<

all: $(KERNEL)

clean:
	rm -f $(OBJS)
	rm -f $(IMAGE)
	rm -f $(KERNEL)

$(IMAGE): $(KERNEL)
	grub-mkrescue rootfs -o $@

$(KERNEL): $(OBJS)
	$(CC) -T linker.ld -o $@ -ffreestanding -nostdlib -lgcc $^
	grub-file --is-x86-multiboot2 $@

%.c.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

%.s.o: %.s
	$(AS) $< -o $@

%.asm.cpp.o: %.asm.cpp
	nasm -felf32 -o $@ $<

%.asm.cpp: %.asm
	cpp -P $< $@
