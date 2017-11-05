BINS = dummy1.bin dummy2.bin

all: $(BINS)

clean:
	rm -f $(BINS)

%.bin: %.asm
	nasm -f bin $< -o $@
