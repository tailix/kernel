SUBDIRS = x86

all:
	make all -C $(ARCH)

clean: $(addprefix clean-, $(SUBDIRS))

clean-%:
	make clean -C $*
