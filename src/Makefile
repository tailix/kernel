CCPREFIX = /home/kotovalexarian/repos/global/tailix/cross/root/bin/i386-elf-

LIBKERNAUX_PREFIX = /opt/libkernaux/i386

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
	-I$(LIBKERNAUX_PREFIX)/include

CPPFLAGS = \
	-DKERNAUX_ENABLE_ASSERT \
	-DKERNAUX_ENABLE_GUARD

# Architecture-dependent
OBJS = start.s.o
OBJS += main.c.o
OBJS += poweroff.c.o
OBJS += panic.c.o
OBJS += paging.c.o

# Architecture-independent
OBJS += info.c.o

# Built-in drivers
OBJS += timer.c.o

OBJS += protected.c.o

OBJS += interrupts/main.asm.cpp.o
OBJS += interrupts/exception.c.o
OBJS += interrupts/hwint.c.o
OBJS += interrupts/syscall.c.o

all: $(KERNEL)

clean:
	rm -f $(KERNEL) $(OBJS)

$(KERNEL): $(OBJS)
	$(CC) -T linker.ld -o $@ $^ -ffreestanding -nostdlib -lkernaux -lgcc -Wl,-L$(LIBKERNAUX_PREFIX)/lib
	grub-file --is-x86-multiboot2 $@

%.c.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

%.s.o: %.s
	$(AS) $< -o $@

%.asm.cpp.o: %.asm.cpp
	nasm -f elf32 -o $@ $<

%.asm.cpp: %.asm
	cpp -P $< $@
