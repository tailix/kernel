OUTPUT = libsrc.a

OBJS = info.o memset.o strlen.o itoa.o strncpy.o

all: $(OUTPUT)

clean:
	rm -f $(OUTPUT) $(OBJS)

$(OUTPUT): $(OBJS)
	$(AR) -rcs $@ $(OBJS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)
