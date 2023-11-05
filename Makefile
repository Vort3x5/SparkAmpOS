ASM := fasm

CC := i686-elf-gcc
CFLAGS := -O2 -g -nostdlib -ffreestanding -Wall -Wextra -I include/

C_SRCS := $(wildcard drivers/*.c src/*.c)
S_SRCS := $(wildcard src/*.S)
ASM_SRCS := $(wildcard src/*.asm)
BOOT_SRCS := $(wildcard boot/*.asm)

C_OBJS := $(patsubst %.c, obj/%.o, $(notdir $(C_SRCS)))
S_OBJS := $(patsubst %.S, obj/%.o, $(notdir $(S_SRCS)))
ASM_OBJS := $(patsubst %.asm, obj/%.o, $(notdir $(ASM_SRCS)))

BOOT_BINS := $(patsubst %.asm, bin/%.bin, $(notdir $(BOOT_SRCS)))

all: dirs Entry SparkAmpOS.bin kernel_info $(BOOT_BINS)
	dd if=/dev/zero of=iso/boot.iso bs=512 count=2880
	dd if=./bin/boot.bin of=iso/boot.iso conv=notrunc bs=512 seek=0 count=1
	dd if=./bin/load.bin of=iso/boot.iso conv=notrunc bs=512 seek=1 count=4
	dd if=./bin/SparkAmpOS.bin of=iso/boot.iso conv=notrunc bs=512 seek=5 count=2048
	# cat bin/boot.bin bin/load.bin bin/SparkAmpOS.bin > iso/boot.iso - not loading second stage

GRUB: dirs Entry SparkAmpOS
	mkdir -p iso/boot/grub
	grub-file --is-x86-multiboot SparkAmpOS
	mv SparkAmpOS boot.iso
	cp boot.iso iso/boot/boot.iso
	./scripts/grub.sh
	grub-mkrescue -o iso/boot.iso iso

bin/%.bin: boot/%.asm
	$(ASM) $< $@

kernel_info:
	lua scripts/kernel_size.lua

SparkAmpOS.bin: $(ASM_OBJS) $(C_OBJS)
	$(CC) $(CFLAGS) -e Main -Ttext 0x2000 -o bin/$@ $^

SparkAmpOS: $(ASM_OBJS) $(C_OBJS)
	$(CC) $(CFLAGS) -e Main -T scripts/link.ld -o $@ $^

obj/%.o: drivers/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

Entry:
	$(ASM) src/entry.asm obj/entry.o

dirs:
	mkdir -p obj/
	mkdir -p bin/
	mkdir -p iso/

clean:
	rm -rf boot.iso bin/ iso/ obj/
	
# for GRUB use cdrom
# qemu-system-i386 -cdrom SparkAmpOS

release:
	qemu-system-i386 -audiodev pa,id=snd0 -machine pcspk-audiodev=snd0 -fda iso/boot.iso

debug:
	# bochs -f .bochsrc
	qemu-system-i386 -audiodev pa,id=snd0 -machine pcspk-audiodev=snd0 -fda iso/boot.iso -s -S &
	gdb -x scripts/gdb.in
	# lldb --source scripts/lldb.in

.PHONY: GRUB clean release debug
