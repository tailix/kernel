CCPREFIX = /home/kotovalexarian/repos/global/tailix/cross/root/bin/i386-elf-

LIBFSC_PREFIX     = /opt/libfsc/i386
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
	-I$(LIBFSC_PREFIX)/include \
	-I$(LIBKERNAUX_PREFIX)/include

CPPFLAGS = \
	-DKERNAUX_ENABLE_ASSERT \
	-DKERNAUX_ENABLE_GUARD

# Architecture-dependent
OBJS = start.s.o
OBJS += main.c.o
OBJS += panic.c.o
OBJS += paging.c.o

# Architecture-independent
OBJS += info.c.o

# Built-in drivers
OBJS += pic.c.o
OBJS += timer.c.o

OBJS += protected.c.o protected.asm.cpp.o

OBJS += tss.c.o tss.asm.cpp.o
OBJS += tasks.asm.cpp.o

OBJS += interrupt.asm.cpp.o

OBJS += exception.c.o
OBJS += hwint.c.o
OBJS += syscall.c.o

all: $(KERNEL)

clean:
	rm -f $(KERNEL) $(OBJS)

$(KERNEL): $(OBJS)
	$(CC) -T linker.ld -o $@ $^ -ffreestanding -nostdlib -lkernaux -lc -lgcc -Wl,-L$(LIBFSC_PREFIX)/lib -Wl,-L$(LIBKERNAUX_PREFIX)/lib
	grub-file --is-x86-multiboot2 $@

%.c.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

%.s.o: %.s
	$(AS) $< -o $@

%.asm.cpp.o: %.asm.cpp
	nasm -f elf32 -o $@ $<

%.asm.cpp: %.asm
	cpp -P $< $@
