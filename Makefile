ASM := fasm

CC := i686-elf-gcc
CFLAGS := -O2 -g -nostdlib -ffreestanding -Wall -Wextra -I include/

C_SRCS := $(wildcard drivers/*.c src/*.c)
S_SRCS := $(wildcard src/*.S)

C_OBJS := $(patsubst %.c, obj/%.o, $(notdir $(C_SRCS)))
S_OBJS := $(patsubst %.S, obj/%.o, $(notdir $(S_SRCS)))

all: dirs SparkAmpOS.bin kernel_info Boot
	dd if=/dev/zero of=iso/boot.iso bs=512 count=2880
	dd if=./Boot of=iso/boot.iso conv=notrunc bs=512 seek=0 count=1
	dd if=./SparkAmpOS.bin of=iso/boot.iso conv=notrunc bs=512 seek=1 count=2048

GRUB: dirs SparkAmpOS
	mkdir -p iso/boot/grub
	grub-file --is-x86-multiboot SparkAmpOS
	cp SparkAmpOS iso/boot/SparkAmpOS
	./scripts/grub.sh
	grub-mkrescue -o SparkAmpOS iso
	qemu-system-i386 -audiodev pa,id=snd0 -machine pcspk-audiodev=snd0 -cdrom SparkAmpOS

Boot:
	$(ASM) boot/boot.asm $@

kernel_info:
	lua scripts/kernel_size.lua

SparkAmpOS.bin: $(C_OBJS)
	$(CC) $(CFLAGS) -e Main -Ttext 0x1000 -o $@ $^

SparkAmpOS: $(S_OBJS) $(C_OBJS)
	$(CC) $(CFLAGS) -e Main -T scripts/link.ld -o $@ $^

obj/%.o: drivers/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

obj/%.o: src/%.S
	$(CC) $(CFLAGS) -c -o $@ $< -D__is_kernel

dirs:
	mkdir -p obj/
	mkdir -p iso/

clean:
	rm -rf SparkAmpOS SparkAmpOS.bin iso/ obj/ Boot

release:
	qemu-system-i386 -cdrom SparkAmpOS

debug:
	bochs -f .bochsrc

.PHONY: GRUB clean release debug
