OUTPUT = libarch.a

OBJS = dummy.c.o

all: $(OUTPUT)

clean:
	rm -f $(OUTPUT) $(OBJS)

$(OUTPUT): $(OBJS)
	$(AR) -rcs $@ $(OBJS)

%.c.o: %.c
	$(CC) -c $< -o $@ -std=gnu99 -ffreestanding -nostdinc -fno-builtin -fno-stack-protector -Wall -Wextra -I "$(INCLUDE)"
