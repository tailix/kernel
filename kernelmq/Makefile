CCPREFIX = i686-elf-

AS = $(CCPREFIX)as
CC = $(CCPREFIX)gcc
LD = $(CCPREFIX)ld

KERNEL = kernelmq.multiboot2

CFLAGS = -std=gnu99 -ffreestanding -nostdinc -fno-builtin -fno-stack-protector -Wall -Wextra -I/usr/local/include

# Architecture-dependent
OBJS = start.s.o
OBJS += main.c.o
OBJS += panic.c.o
OBJS += paging.c.o
OBJS += page_dir.c.o

# Architecture-independent
OBJS += info.c.o
OBJS += process.c.o

# Built-in drivers
OBJS += console.c.o
OBJS += pic.c.o
OBJS += timer.c.o

# For debugging
OBJS += logger.c.o

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
	$(CC) -T linker.ld -o $@ $^ -ffreestanding -nostdlib -lgcc -lkernaux -Wl,-L/usr/local/lib
	grub-file --is-x86-multiboot2 $@

%.c.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

%.s.o: %.s
	$(AS) $< -o $@

%.asm.cpp.o: %.asm.cpp
	nasm -f elf32 -o $@ $<

%.asm.cpp: %.asm
	cpp -P $< $@
