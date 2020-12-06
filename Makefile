GRUBCFG = rootfs/boot/grub/grub.cfg
KERNEL  = rootfs/boot/kernelmq.multiboot2

IMAGE = image.iso

.PHONY: kernelmq/kernelmq.multiboot2

run: $(IMAGE)
	qemu-system-i386 -cdrom $< -display none -serial stdio

clean:
	rm -f $(KERNEL)
	make -C kernelmq clean

$(IMAGE): $(GRUBCFG) $(KERNEL)
	grub-mkrescue rootfs -o $@

$(KERNEL): kernelmq/kernelmq.multiboot2
	cp $< $@

kernelmq/kernelmq.multiboot2:
	make -C kernelmq kernelmq.multiboot2
