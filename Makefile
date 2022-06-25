IMAGE = image.iso
ROOTFS = rootfs

GRUBCFG = $(ROOTFS)/boot/grub/grub.cfg
KERNEL  = $(ROOTFS)/boot/tailix.multiboot2

.PHONY: src/tailix.multiboot2

all: run0

run0: $(IMAGE)
	qemu-system-i386 -cdrom $< -serial stdio -display none

run1: $(IMAGE)
	qemu-system-i386 -cdrom $< -serial stdio

clean:
	rm -f $(IMAGE) $(KERNEL)
	make -C src clean

$(IMAGE): $(GRUBCFG) $(KERNEL)
	grub-mkrescue $(ROOTFS) -o $@

$(KERNEL): src/tailix.multiboot2
	cp $< $@

src/tailix.multiboot2:
	make -C src tailix.multiboot2
