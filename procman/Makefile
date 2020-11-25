PROCMAN = procman

$(PROCMAN): main.asm
	nasm -f bin $< -o $@

clean:
	rm -f $(PROCMAN)
