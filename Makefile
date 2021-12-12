GRUBCFG = rootfs/boot/grub/grub.cfg
KERNEL  = rootfs/boot/tailix.multiboot2

IMAGE = image.iso

.PHONY: kernelmq/tailix.multiboot2

run: $(IMAGE)
	qemu-system-i386 -cdrom $< -display none -serial stdio

clean:
	rm -f $(KERNEL)
	make -C kernelmq clean

$(IMAGE): $(GRUBCFG) $(KERNEL)
	grub-mkrescue rootfs -o $@

$(KERNEL): kernelmq/tailix.multiboot2
	cp $< $@

kernelmq/tailix.multiboot2:
	make -C kernelmq tailix.multiboot2
