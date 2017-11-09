export ARCH = x86
export CCPREFIX = i686-elf-

export AR = $(CCPREFIX)ar
export AS = $(CCPREFIX)as
export CC = $(CCPREFIX)gcc

export INCLUDE = $(shell pwd)/include
export KERNEL  = $(shell pwd)/rootfs/boot/kernelmq.multiboot2

export CFLAGS = -std=gnu99 -ffreestanding -nostdinc -fno-builtin -fno-stack-protector -Wall -Wextra -I $(INCLUDE)

SUBDIRS = arch

IMAGE = $(shell pwd)/image.iso

run: all-arch
	grub-file --is-x86-multiboot2 $(KERNEL)
	grub-mkrescue rootfs -o $(IMAGE)
	qemu-system-i386 -cdrom $(IMAGE)

all: all-arch

clean: $(addprefix clean-, $(SUBDIRS))
	rm -f $(IMAGE)
	rm -f $(KERNEL)

########
# arch #
########

all-arch:
	make all -C arch

clean-arch:
	make clean -C arch
