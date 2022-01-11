CCPREFIX = i386-elf-

AS = $(CCPREFIX)as
CC = $(CCPREFIX)gcc
LD = $(CCPREFIX)ld

MEMGR = memgr

CFLAGS = -std=gnu99 -ffreestanding -nostdinc -fno-builtin -fno-stack-protector -Wall -Wextra

all: $(MEMGR)

clean:
	rm -f $(MEMGR) start.o

$(MEMGR): start.o
	$(LD) -o $@ -T linker.ld $^

start.o: start.c
	$(CC) -c $< -o $@ $(CFLAGS)
