CCPREFIX = i686-elf-

AR = $(CCPREFIX)ar
AS = $(CCPREFIX)as
CC = $(CCPREFIX)gcc

MEMGR = memgr

CFLAGS = -std=gnu99 -ffreestanding -nostdinc -fno-builtin -fno-stack-protector -Wall -Wextra

all: $(MEMGR)

clean:
	rm -f $(MEMGR) start.o

$(MEMGR): start.o
	$(CC) -T linker.ld -o $@ -ffreestanding -nostdlib -lgcc $^

start.o: start.c
	$(CC) -c $< -o $@ $(CFLAGS)
