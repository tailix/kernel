all: run

GRUBCFG = rootfs/boot/grub/grub.cfg
KERNEL  = rootfs/boot/tailix.multiboot2

IMAGE = image.iso

.PHONY: kernel/tailix.multiboot2

run: run0

run0: $(IMAGE)
	qemu-system-i386 -cdrom $< -serial stdio -display none

run1: $(IMAGE)
	qemu-system-i386 -cdrom $< -serial stdio

clean:
	rm -f $(IMAGE) $(KERNEL)
	make -C kernel clean

$(IMAGE): $(GRUBCFG) $(KERNEL)
	grub-mkrescue rootfs -o $@

$(KERNEL): kernel/tailix.multiboot2
	cp $< $@

kernel/tailix.multiboot2:
	make -C kernel tailix.multiboot2
