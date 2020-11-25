CCPREFIX = i686-elf-

AR = $(CCPREFIX)ar
AS = $(CCPREFIX)as
CC = $(CCPREFIX)gcc

PROCMAN = procman

CFLAGS = -std=gnu99 -ffreestanding -nostdinc -fno-builtin -fno-stack-protector -Wall -Wextra

all: $(PROCMAN)

clean:
	rm -f $(PROCMAN) start.o

$(PROCMAN): start.o
	$(CC) -T linker.ld -o $@ -ffreestanding -nostdlib -lgcc $^

start.o: start.c
	$(CC) -c $< -o $@ $(CFLAGS)
