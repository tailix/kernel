include config.mk

export AR = $(CCPREFIX)ar
export AS = $(CCPREFIX)as
export CC = $(CCPREFIX)gcc

export INCLUDE = $(shell pwd)/include
export KERNEL  = $(shell pwd)/kernelmq

export LIBSRC  = $(shell pwd)/src/libsrc.a
export LIBK    = $(shell pwd)/libk/libk.a

export MODULES = $(addprefix $(shell pwd)/modules/, dummy1.bin dummy2.bin)

export CFLAGS = -std=gnu99 -ffreestanding -nostdinc -fno-builtin -fno-stack-protector -Wall -Wextra -I $(INCLUDE)

ifeq (none, $(ARCH))
run: test
else
run: run-iso
endif

all: all-kernel

clean: clean-kernel clean-iso clean-libk clean-arch clean-test clean-modules

test: run-test

##########
# Kernel #
##########

all-kernel: all-arch
	cp arch/$(ARCH)/kernelmq $(KERNEL)

clean-kernel:
	rm -f $(KERNEL)

#######
# src #
#######

all-src:
	make all -C src

clean-src:
	make clean -C src

########
# arch #
########

all-arch: all-src all-libk
	make all -C arch

clean-arch:
	make clean -C arch

#######
# ISO #
#######

run-iso: all-iso
	make run -C iso

all-iso: all-kernel all-modules
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
