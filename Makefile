run: all
	qemu-system-i386 -kernel arch/kernel -d guest_errors

all:
	make all -C arch I=$(shell pwd)/include

clean:
	make clean -C arch I=$(shell pwd)/include
