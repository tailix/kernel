CCPREFIX = i686-elf-

AR = $(CCPREFIX)ar
AS = $(CCPREFIX)as
CC = $(CCPREFIX)gcc

MEMGR = memgr

all: $(MEMGR)

clean:
	rm -f $(MEMGR) main.o

$(MEMGR): main.o
	$(CC) -T linker.ld -o $@ -ffreestanding -nostdlib -lgcc $^

main.o: main.asm
	nasm -f elf32 $< -o $@
