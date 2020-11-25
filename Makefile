GRUBCFG = rootfs/boot/grub/grub.cfg
KERNEL  = rootfs/boot/kernelmq.multiboot2
PROCMAN = rootfs/boot/procman
MEMGR   = rootfs/boot/memgr

IMAGE = image.iso

run: $(IMAGE)
	qemu-system-i386 -cdrom $< -display none -serial stdio

clean:
	rm -f $(KERNEL) $(PROCMAN) $(MEMGR)
	make -C kernelmq clean
	make -C procman clean

$(IMAGE): $(GRUBCFG) $(KERNEL) $(PROCMAN) $(MEMGR)
	grub-mkrescue rootfs -o $@

$(KERNEL): kernelmq/kernelmq.multiboot2
	cp $< $@

$(PROCMAN):
	make -C procman procman
	cp procman/procman $@

$(MEMGR):
	make -C memgr memgr
	cp memgr/memgr $@

kernelmq/kernelmq.multiboot2:
	make -C kernelmq kernelmq.multiboot2
