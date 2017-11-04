SUITES = libk

run:   $(addprefix run-,   $(SUITES))
all:   $(addprefix all-,   $(SUITES))
clean: $(addprefix clean-, $(SUITES))

run-%: all-%
	make run -C $*

all-%:
	make all -C $*

clean-%:
	make clean -C $*
