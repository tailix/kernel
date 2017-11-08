OBJS = main.o

all: $(OUTPUT)

clean:
	rm -f $(OUTPUT) $(OBJS)

$(OUTPUT): $(OBJS)
	$(CC) -o $(OUTPUT) $(OBJS) $(LIBSRC) $(LIBK)

%.o: %.c
	$(CC) -c $< -o $@ -std=gnu99 -Wall -Wextra -I $(INCLUDE)
