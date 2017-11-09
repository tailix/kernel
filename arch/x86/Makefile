# Architecture-dependent
OBJS = start.s.o
OBJS += main.c.o
OBJS += init.c.o
OBJS += multiboot.c.o
OBJS += panic.c.o  panic.asm.cpp.o
OBJS += pfa.c.o
OBJS += paging.c.o paging.asm.cpp.o
OBJS += pagedir.c.o

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

all: $(KERNEL)

clean:
	rm -f $(OBJS)

$(KERNEL): $(OBJS)
	$(CC) -T linker.ld -o $(KERNEL) -ffreestanding -nostdlib -lgcc $(OBJS) $(LIBSRC) $(LIBK)

%.c.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

%.s.o: %.s
	$(AS) $< -o $@

%.asm.cpp.o: %.asm.cpp
	nasm -felf32 -o $@ $<

%.asm.cpp: %.asm
	cpp -P $< $@
