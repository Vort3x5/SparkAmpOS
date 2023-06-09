OS.img:
	fasm boot/boot.asm bin/boot.bin
	fasm boot/load.asm bin/load.bin
	cargo rustc -- -C link-arg=-nostartfiles
	mv target/debug/SparkAmpOS bin/
	cat bin/boot.bin bin/load.bin bin/SparkAmpOS > bin/OS.img

clean:
	cargo clean
	rm bin/*

run:
	bochs -f .bochsrc
	# qemu-system-i386 -fda build/main_floppy.img
