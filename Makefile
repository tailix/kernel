run: run-iso

all: all-kernel all-iso

clean: clean-iso clean-kernel

all-kernel:
	make all -C arch I=$(shell pwd)/include

clean-kernel:
	make clean -C arch

run-iso: all-kernel
	make run -C iso K=$(shell pwd)/arch/kernel

all-iso: all-kernel
	make all -C iso K=$(shell pwd)/arch/kernel

clean-iso:
	make clean -C iso
