OBJS = dummy.c.o

all: $(OUTPUT)

clean:
	rm -f $(OUTPUT) $(OBJS)

$(OUTPUT): $(OBJS)
	$(CC) -T linker.ld -o $(OUTPUT) -ffreestanding -nostdlib -lgcc $(OBJS) $(LIBSRC) $(LIBK)

%.c.o: %.c
	$(CC) -c $< -o $@ -std=gnu99 -ffreestanding -nostdinc -fno-builtin -fno-stack-protector -Wall -Wextra -I "$(INCLUDE)"
