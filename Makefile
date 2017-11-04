include config.mk

export INCLUDE = $(shell pwd)/include
export KERNEL  = $(shell pwd)/arch/kernel
export LIBK    = $(shell pwd)/libk/libk.a

run: run-iso

all:     all-kernel   all-iso   all-libk   all-test
clean: clean-kernel clean-iso clean-libk clean-test

all-kernel: all-libk
	make all -C arch

clean-kernel:
	make clean -C arch

run-iso: all-kernel
	make run -C iso

all-iso: all-kernel
	make all -C iso

clean-iso:
	make clean -C iso

all-libk:
	make all -C libk

clean-libk:
	make clean -C libk

test-libk: all-libk
	make run -C test/libk

clean-test:
	make clean -C test/libk
