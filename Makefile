IMAGE = image.iso
ROOTFS = rootfs

GRUBCFG = $(ROOTFS)/boot/grub/grub.cfg
KERNEL  = $(ROOTFS)/boot/tailix.multiboot2
PROCMAN = $(ROOTFS)/boot/procman
MEMGR   = $(ROOTFS)/boot/memgr

.PHONY: kernel/tailix.multiboot2 procman/procman memgr/memgr

all: run0

run0: $(IMAGE)
	qemu-system-i386 -cdrom $< -serial stdio -display none

run1: $(IMAGE)
	qemu-system-i386 -cdrom $< -serial stdio

clean:
	rm -f $(IMAGE) $(KERNEL) $(PROCMAN) $(MEMGR)
	make -C kernel clean
	make -C procman clean
	make -C memgr clean

$(IMAGE): $(GRUBCFG) $(KERNEL) $(PROCMAN) $(MEMGR)
	grub-mkrescue $(ROOTFS) -o $@

$(KERNEL): kernel/tailix.multiboot2
	cp $< $@

$(PROCMAN): procman/procman
	cp $< $@

$(MEMGR): memgr/memgr
	cp $< $@

kernel/tailix.multiboot2:
	make -C kernel tailix.multiboot2

procman/procman:
	make -C procman procman

memgr/memgr:
	make -C memgr memgr
