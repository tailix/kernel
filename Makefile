run: all
	qemu-system-i386 -kernel src/kernel -d guest_errors

all:
	make all -C src

clean:
	make clean -C src
