CCPREFIX = i686-elf-

AS = $(CCPREFIX)as
CC = $(CCPREFIX)gcc
LD = $(CCPREFIX)ld

PROCMAN = procman

CFLAGS = -std=gnu99 -ffreestanding -nostdinc -fno-builtin -fno-stack-protector -Wall -Wextra

all: $(PROCMAN)

clean:
	rm -f $(PROCMAN) start.o

$(PROCMAN): start.o
	$(LD) -o $@ -T linker.ld $^

start.o: start.c
	$(CC) -c $< -o $@ $(CFLAGS)
