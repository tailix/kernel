include config.mk

export AR = $(CCPREFIX)ar
export AS = $(CCPREFIX)as
export CC = $(CCPREFIX)gcc

export INCLUDE = $(shell pwd)/include
export KERNEL  = $(shell pwd)/rootfs/boot/kernelmq.multiboot2

export LIBSRC  = $(shell pwd)/src/libsrc.a
export LIBK    = $(shell pwd)/libk/libk.a

export CFLAGS = -std=gnu99 -ffreestanding -nostdinc -fno-builtin -fno-stack-protector -Wall -Wextra -I $(INCLUDE)

SUBDIRS = arch libk src

IMAGE = $(shell pwd)/image.iso

run: all-kernel
	grub-file --is-x86-multiboot2 $(KERNEL)
	grub-mkrescue rootfs -o $(IMAGE)
	qemu-system-i386 -cdrom $(IMAGE)

all: all-kernel

clean: clean-kernel $(addprefix clean-, $(SUBDIRS))
	rm -f $(IMAGE)

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

########
# libk #
########

all-libk:
	make all -C libk

clean-libk:
	make clean -C libk
