run: all
	qemu-system-i386 -cdrom image.iso

all:
	grub-file --is-x86-multiboot2 "$(KERNEL)"
	cp "$(KERNEL)" rootfs/boot/kernelmq
	grub-mkrescue rootfs -o image.iso

clean:
	rm -f image.iso rootfs/boot/kernelmq
