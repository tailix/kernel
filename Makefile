GRUBCFG = rootfs/boot/grub/grub.cfg
KERNEL  = rootfs/boot/kernelmq.multiboot2
PROCMAN = rootfs/boot/procman
MEMGR   = rootfs/boot/memgr

IMAGE = image.iso

.PHONY: kernelmq/kernelmq.multiboot2 procman/procman memgr/memgr

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

$(PROCMAN): procman/procman
	cp $< $@

$(MEMGR): memgr/memgr
	cp $< $@

kernelmq/kernelmq.multiboot2:
	make -C kernelmq kernelmq.multiboot2

procman/procman:
	make -C procman procman

memgr/memgr:
	make -C memgr memgr
