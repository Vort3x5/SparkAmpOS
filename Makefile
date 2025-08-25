ASM := fasm
CC := gcc
LD := ld
CFLAGS := -m32 -O0 -std=gnu99 -nostdlib -ffreestanding -fno-pie -fno-stack-protector \
          -I include/ -Wno-shift-count-overflow -Wno-int-to-pointer-cast \
		  -fno-builtin -fno-asynchronous-unwind-tables -g3

C_SRCS := $(wildcard src/*.c drivers/*.c lib/*.c)
ASM_SRCS := $(wildcard init/*.asm)
BOOT_SRCS := $(wildcard boot/src/*.asm)

C_OBJS := $(patsubst %.c, obj/%.o, $(notdir $(C_SRCS)))
ASM_OBJS := $(patsubst %.asm, obj/%.o, $(notdir $(ASM_SRCS)))

BOOT_BINS := $(patsubst %.asm, bin/%.bin, $(notdir $(BOOT_SRCS)))

LLD := $(ASM_OBJS) $(C_OBJS)

DRIVE := /dev/sdb

all: dirs SparkAmpOS.bin kernel_info $(BOOT_BINS)
	dd if=./bin/boot.bin of=$(DRIVE) bs=512 count=1
	dd if=./bin/load.bin of=$(DRIVE) bs=512 seek=1
	dd if=./bin/SparkAmpOS.bin of=$(DRIVE) bs=512 seek=3

floppy: dirs SparkAmpOS.bin kernel_info $(BOOT_BINS)
	dd if=./bin/boot.bin of=iso/boot.iso bs=512 count=1
	dd if=./bin/load.bin of=iso/boot.iso bs=512 seek=1
	dd if=./bin/SparkAmpOS.bin of=iso/boot.iso bs=512 seek=3

GRUB: dirs SparkAmpOS
	mkdir -p iso/boot/grub
	grub-file --is-x86-multiboot bin/SparkAmpOS
	cp bin/SparkAmpOS iso/boot/boot.iso
	./scripts/grub.sh
	grub-mkrescue -o iso/boot.iso iso
	qemu-system-i386 -audio driver=alsa,model=ac97,id=alsa -cdrom iso/boot.iso -s -S &
	gdb -x scripts/db_grub.gdb

bin/%.bin: boot/src/%.asm
	$(ASM) $< $@

kernel_info:
	lua scripts/kernel_size.lua

# Clean Binary for the custom bootloader without debugging symbols
SparkAmpOS.bin: $(ASM_OBJS) $(C_OBJS)
	$(LD) -m elf_i386 -T scripts/link.ld --oformat binary -o bin/$@ $(LLD)

# ELF for the custom bootloader with debugging symbols
SparkAmpOS.elf: $(ASM_OBJS) $(C_OBJS)
	$(LD) -m elf_i386 -T scripts/link.ld -o bin/$@ $(LLD)

# Binary for GRUB with the debugging symbols
SparkAmpOS: $(ASM_OBJS) $(C_OBJS)
	$(CC) $(CFLAGS) -T scripts/grub.ld -o bin/$@ $(LLD)

obj/%.o: lib/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

obj/%.o: drivers/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

obj/%.o: init/%.asm
	$(ASM) $< $@

dirs:
	mkdir -p obj/
	mkdir -p bin/
	mkdir -p iso/

clean:
	rm -rf boot.iso bin/ iso/ obj/
	
# for GRUB use cdrom
# qemu-system-i386 -cdrom iso/boot.iso
#
# for floppy
# qemu-system-i386 -audiodev pa,id=snd0 -machine pcspk-audiodev=snd0 -fda iso/boot.iso

# sudo chown <username> $(DRIVE)
release:
	qemu-system-i386 -audio driver=alsa,model=ac97,id=alsa -hdb $(DRIVE)
	# qemu-system-i386 -audio driver=alsa,model=ac97,id=alsa -cdrom iso/boot.iso

# bochs -f .bochsrc
debug: dirs SparkAmpOS.bin SparkAmpOS.elf kernel_info $(BOOT_BINS)
	qemu-system-i386 -audio driver=alsa,model=ac97,id=alsa -hdb $(DRIVE) -s -S &
	gdb -x scripts/db_input.gdb
	# qemu-system-i386 -cdrom iso/boot.iso

.PHONY: GRUB clean release debug
