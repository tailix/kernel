CCPREFIX = i686-elf-

AS = $(CCPREFIX)as
CC = $(CCPREFIX)gcc

OBJS = boot.s.o main.c.o

all: kernel

clean:
	rm kernel $(OBJS)

kernel: $(OBJS)
	$(CC) -T linker.ld -o $@ -ffreestanding -nostdlib -O2 -lgcc $(OBJS)

%.c.o: %.c
	$(CC) -c $< -o $@ -std=c99 -ffreestanding -O2 -Wall -Wextra

%.s.o: %.s
	$(AS) $< -o $@
