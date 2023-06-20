OS.img:
	fasm boot/boot.asm bin/boot.bin
	# fasm src/main.asm bin/SparkAmpOS
	gcc -ffreestanding -I include/ -c src/main.c -o obj/SparkAmpOS.o 
	ld -i -e _main -Ttext 0x1000 obj/SparkAmpOS.o -o obj/SparkAmpOS_temp.o
	mv obj/SparkAmpOS_temp.o obj/SparkAmpOS.o
	objcopy -R .note -R .comment -S -O binary obj/SparkAmpOS.o bin/SparkAmpOS
	# cargo rustc -- -C link-arg=-nostartfiles
	# mv target/debug/SparkAmpOS bin/
	dd if=/dev/zero of=OS.img bs=512 count=2880
	dd if=./bin/boot.bin of=OS.img conv=notrunc bs=512 seek=0 count=1
	dd if=./bin/SparkAmpOS of=OS.img conv=notrunc bs=512 seek=1 count=2048
	# cat bin/boot.bin bin/SparkAmpOS > OS.img
	lua scripts/file_size.lua

clean:
	cargo clean
	rm bin/*
	rm OS.img
	rm obj/*

release:
	qemu-system-i386 -drive format=raw,file=OS.img

debug:
	bochs -f .bochsrc
