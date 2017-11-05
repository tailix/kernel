include config.mk

export INCLUDE = $(shell pwd)/include
export KERNEL  = $(shell pwd)/arch/kernel
export LIBK    = $(shell pwd)/libk/libk.a

run: run-iso

all:     all-kernel   all-iso   all-libk   all-test   all-modules
clean: clean-kernel clean-iso clean-libk clean-test clean-modules

test: run-test

##########
# Kernel #
##########

all-kernel: all-libk
	make all -C arch

clean-kernel:
	make clean -C arch

#######
# ISO #
#######

run-iso: all-iso
	make run -C iso

all-iso: all-kernel
	make all -C iso

clean-iso:
	make clean -C iso

########
# libk #
########

all-libk:
	make all -C libk

clean-libk:
	make clean -C libk

#########
# Tests #
#########

run-test: all-test
	make run -C test

all-test: all-libk
	make all -C test

clean-test:
	make clean -C test

###########
# Modules #
###########

all-modules:
	make all -C modules

clean-modules:
	make clean -C modules
