ifneq (ok, $(shell test -f '$(K)' && echo 'ok'))
$(error 'Kernel "$(K)" does not exist')
endif

ifneq (ok, $(shell grub-file --is-x86-multiboot '$(K)' && echo 'ok'))
$(error 'Kernel "$(K)" is not compliant with Multiboot 1')
endif

ifneq (ok, $(shell grub-file --is-x86-multiboot2 '$(K)' && echo 'ok'))
$(error 'Kernel "$(K)" is not compliant with Multiboot 2')
endif

run: all
	qemu-system-i386 -cdrom image.iso

all:
	cp "$(K)" rootfs/boot/kernelmq
	grub-mkrescue rootfs -o image.iso

clean:
	rm -f image.iso rootfs/boot/kernelmq
