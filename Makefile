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

SUBDIRS = arch iso libk modules src

run: run-iso

all: all-kernel

clean: clean-kernel $(addprefix clean-, $(SUBDIRS))

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

###########
# Modules #
###########

all-modules:
	make all -C modules

clean-modules:
	make clean -C modules
