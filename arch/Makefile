export OUTPUT = kernel

ARCHES = $(shell ls -d */)
ARCHES := $(ARCHES:/=)

all:
	make all -C $(ARCH)

clean: $(addprefix clean-, $(ARCHES))

clean-%:
	make clean -C $*
