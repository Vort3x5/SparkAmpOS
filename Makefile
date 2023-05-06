boot.bin:
	fasm boot/BootSector.asm bin/BootSector.bin
	fasm boot/Snd.asm bin/Snd.bin

clean:
	rm bin/*

run:
	dd if=bin/BootSector.bin of=bin/OS.img bs=512 count=1 conv=notrunc
	dd if=bin/Snd.bin of=bin/OS.img bs=512 seek=1 conv=notrunc
	bochs
