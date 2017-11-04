include config.mk

run: run-iso

all:     all-kernel   all-iso   all-libk   all-test
clean: clean-kernel clean-iso clean-libk clean-test

all-kernel: all-libk
	make all -C arch I=$(shell pwd)/include LIBK=$(shell pwd)/libk/libk.a

clean-kernel:
	make clean -C arch

run-iso: all-kernel
	make run -C iso K=$(shell pwd)/arch/kernel

all-iso: all-kernel
	make all -C iso K=$(shell pwd)/arch/kernel

clean-iso:
	make clean -C iso

all-libk:
	make all -C libk

clean-libk:
	make clean -C libk

test-libk: all-libk
	make run -C test/libk I=$(shell pwd)/include LIBK=$(shell pwd)/libk/libk.a

clean-test:
	make clean -C test/libk
