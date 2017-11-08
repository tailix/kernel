include config.mk

export AR = $(CCPREFIX)ar
export AS = $(CCPREFIX)as
export CC = $(CCPREFIX)gcc

export INCLUDE = $(shell pwd)/include
export KERNEL  = $(shell pwd)/kernelmq.multiboot2
export LIBK    = $(shell pwd)/libk/libk.a
export LIBARCH = $(shell pwd)/arch/$(ARCH)/libarch.a
export LINKER  = $(shell pwd)/arch/$(ARCH)/linker.ld
export MODULES = $(addprefix $(shell pwd)/modules/, dummy1.bin dummy2.bin)

run: run-iso

all:     all-kernel   all-iso   all-libk   all-arch   all-test   all-modules
clean: clean-kernel clean-iso clean-libk clean-arch clean-test clean-modules

test: run-test

##########
# Kernel #
##########

all-kernel: all-arch all-libk
	$(CC) -T $(LINKER) -o $(KERNEL) -ffreestanding -nostdlib -lgcc $(LIBARCH) $(LIBK)
	grub-file --is-x86-multiboot2 $(KERNEL)

clean-kernel:
	rm -f $(KERNEL)

########
# arch #
########

all-arch:
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
