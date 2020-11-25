GRUBCFG = rootfs/boot/grub/grub.cfg
KERNEL  = rootfs/boot/kernelmq.multiboot2
PROCMAN = rootfs/boot/procman

IMAGE = image.iso

run: $(IMAGE)
	qemu-system-i386 -cdrom $< -display none -serial stdio

clean:
	rm -f $(KERNEL) $(PROCMAN)
	make -C kernelmq clean
	make -C procman clean

$(IMAGE): $(GRUBCFG) $(KERNEL) $(PROCMAN)
	grub-mkrescue rootfs -o $@

$(KERNEL): $(OBJS)
	$(CC) -T linker.ld -o $@ -ffreestanding -nostdlib -lgcc $^
	grub-file --is-x86-multiboot2 $@

$(KERNEL): kernelmq/kernelmq.multiboot2
	cp $< $@

$(PROCMAN): procman/procman
	cp $< $@

kernelmq/kernelmq.multiboot2:
	make -C kernelmq kernelmq.multiboot2

procman/procman:
	make -C procman procman
