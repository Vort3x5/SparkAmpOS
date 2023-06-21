ASM := fasm

CC := gcc
CFLAGS := -ffreestanding -I include/

LD := ld

BOOTDIR := boot
SRCDIR := src
DRIVERSDIR := drivers

OBJDIR := obj
BINDIR := bin
ISODIR := iso

C_SRCS := $(wildcard $(DRIVERSDIR)/*.c $(SRCDIR)/*.c)
OBJ := ${C_SRCS:.c=.o}

all: iso kernel_info

bootloader:
	$(ASM) $(BOOTDIR)/boot.asm $(BINDIR)/boot.bin

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)
	mv $@ obj/

SparkAmpOS: $(OBJ)
	$(CC) $(CFLAGS) -c src/main.c -o obj/SparkAmpOS.o 
	$(LD) -i -e _main -Ttext 0x1000 obj/SparkAmpOS.o -o obj/SparkAmpOS_temp.o
	mv obj/SparkAmpOS_temp.o obj/SparkAmpOS.o
	objcopy -R .note -R .comment -S -O binary obj/SparkAmpOS.o bin/SparkAmpOS

iso: bootloader SparkAmpOS
	dd if=/dev/zero of=iso/boot.iso bs=512 count=2880
	dd if=./bin/boot.bin of=iso/boot.iso conv=notrunc bs=512 seek=0 count=1
	dd if=./bin/SparkAmpOS of=iso/boot.iso conv=notrunc bs=512 seek=1 count=2048

kernel_info:
	lua scripts/file_size.lua

# rust:
# cargo rustc -- -C link-arg=-nostartfiles
# mv target/debug/SparkAmpOS bin/

clean:
	cargo clean
	rm bin/*
	rm iso/boot.iso
	rm obj/*

release:
	qemu-system-i386 -drive format=raw,file=iso/boot.iso

debug:
	bochs -f .bochsrc
