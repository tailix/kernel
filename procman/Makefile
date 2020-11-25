CCPREFIX = i686-elf-

AR = $(CCPREFIX)ar
AS = $(CCPREFIX)as
CC = $(CCPREFIX)gcc

PROCMAN = procman

all: $(PROCMAN)

clean:
	rm -f $(PROCMAN) main.o

$(PROCMAN): main.o
	$(CC) -T linker.ld -o $@ -ffreestanding -nostdlib -lgcc $^

main.o: main.asm
	nasm -f elf32 $< -o $@
