OUTPUT = libsrc.a

OBJS = info.c.o memset.c.o strlen.c.o itoa.c.o strncpy.c.o

all: $(OUTPUT)

clean:
	rm -f $(OUTPUT) $(OBJS)

$(OUTPUT): $(OBJS)
	$(AR) -rcs $@ $(OBJS)

%.c.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)
