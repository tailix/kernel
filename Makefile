run: all
	qemu-system-i386 -kernel arch/kernel -d guest_errors

all:
	make all -C arch

clean:
	make clean -C arch
