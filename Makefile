run: run-iso

all:     all-kernel   all-iso   all-libk
clean: clean-kernel clean-iso clean-libk

all-kernel: all-libk
	make all -C arch LIBK=$(shell pwd)/libk/libk.a

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
