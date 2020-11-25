MEMGR = memgr

$(MEMGR): main.asm
	nasm -f bin $< -o $@

clean:
	rm -f $(MEMGR)
