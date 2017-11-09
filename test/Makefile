SUBDIRS = libk

run:   $(addprefix run-,   $(SUBDIRS))
all:   $(addprefix all-,   $(SUBDIRS))
clean: $(addprefix clean-, $(SUBDIRS))

run-%: all-%
	make run -C $*

all-%:
	make all -C $*

clean-%:
	make clean -C $*
