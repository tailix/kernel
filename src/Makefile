CCPREFIX = /home/kotovalexarian/repos/global/tailix/cross/root/bin/i386-elf-

PREFIX = /opt/freestanding/i386-elf

AS = $(CCPREFIX)as
CC = $(CCPREFIX)gcc
LD = $(CCPREFIX)ld

KERNEL = tailix.multiboot2

CFLAGS = \
	-std=gnu99           \
	-pedantic            \
	-Wall                \
	-Wextra              \
	-ffreestanding       \
	-fno-builtin         \
	-fno-stack-protector \
	-I$(PREFIX)/include

CPPFLAGS = \
	-DKERNAUX_DEBUG \
	-DKERNAUX_BITFIELDS

# Architecture-dependent
OBJS = start.s.o
OBJS += main.c.o
OBJS += panic.c.o
OBJS += paging.c.o

# Architecture-independent
OBJS += info.c.o

OBJS += multiboot2.c.o
OBJS += protected.c.o
OBJS += interrupts.c.o interrupts.asm.cpp.o
OBJS += heap.c.o

all: $(KERNEL)

clean:
	rm -f $(KERNEL) $(OBJS)

$(KERNEL): $(OBJS)
	$(CC) -T linker.ld -o $@ $^ -ffreestanding -nostdlib -ldrivers -lkernaux -lgcc -Wl,-L$(PREFIX)/lib
	grub-file --is-x86-multiboot2 $@

%.c.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

%.s.o: %.s
	$(AS) $< -o $@

%.asm.cpp.o: %.asm.cpp
	nasm -f elf32 -o $@ $<

%.asm.cpp: %.asm
	cpp -P $< $@
