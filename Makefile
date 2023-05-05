ASM = fasm
BINDIR = bin/

boot.bin:
	$(ASM) boot/boot.asm $(BINDIR)boot.bin

clean:
	rm $(BINDIR)*
