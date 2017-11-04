CC = $(CCPREFIX)gcc

CFLAGS = -std=gnu99 -Wall -Wextra

BINS = strlen.bin memset.bin

run: all $(addprefix run-, $(BINS))

all: $(BINS)

clean:
	rm -f $(BINS)

run-%.bin: %.bin
	@./$< && echo "[ OK ] $*" || echo "[FAIL] $*"

%.bin: %.c
	$(CC) $< -o $@ $(CFLAGS) -I "$(INCLUDE)" "$(LIBK)"
