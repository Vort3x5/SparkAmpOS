ASM := fasm
CC := i686-elf-gcc
CFLAGS := -O2 -g -nostdlib -ffreestanding -Wall -Wextra -D__is_kernel -I include/
# CFLAGS := -O2 -nostdlib -ffreestanding -Wall -Wextra -D__is_kernel -I include/

C_SRCS := $(wildcard drivers/*.c src/*.c)
ASM_SRCS := $(wildcard init/*.asm)
BOOT_SRCS := $(wildcard boot/src/*.asm)

C_OBJS := $(patsubst %.c, obj/%.o, $(notdir $(C_SRCS)))
ASM_OBJS := \
	$(patsubst %.asm, obj/%.o, $(notdir $(ASM_SRCS))) \
	obj/crtbegin.o obj/crtend.o \

BOOT_BINS := $(patsubst %.asm, bin/%.bin, $(notdir $(BOOT_SRCS)))

LINK_LIST := \
	obj/crti.o obj/crtbegin.o \
	obj/entry.o $(C_OBJS) \
	obj/crtend.o obj/crtn.o

LLD := obj/entry.o $(C_OBJS)

all: dirs SparkAmpOS.bin kernel_info $(BOOT_BINS)
	dd if=/dev/zero of=iso/boot.iso bs=512 count=2880
	dd if=./bin/boot.bin of=iso/boot.iso conv=notrunc bs=512 seek=0 count=1
	dd if=./bin/load.bin of=iso/boot.iso conv=notrunc bs=512 seek=1 count=2
	dd if=./bin/SparkAmpOS.bin of=iso/boot.iso conv=notrunc bs=512 seek=3 count=100
	# cat bin/boot.bin bin/load.bin bin/SparkAmpOS.bin > iso/boot.iso # - not loading second stage

GRUB: dirs SparkAmpOS
	mkdir -p iso/boot/grub
	grub-file --is-x86-multiboot bin/SparkAmpOS
	cp bin/SparkAmpOS iso/boot/boot.iso
	./scripts/grub.sh
	grub-mkrescue -o iso/boot.iso iso

bin/%.bin: boot/src/%.asm
	$(ASM) $< $@

kernel_info:
	lua scripts/kernel_size.lua

# consider concatenating only
# $(CC) $(CFLAGS) -e _Start -Ttext 0x1000 -o bin/$@ $^
SparkAmpOS.bin: $(ASM_OBJS) $(C_OBJS)
	$(CC) $(CFLAGS) -T scripts/link.ld -o bin/$@ $(LINK_LIST)

SparkAmpOS: $(ASM_OBJS) $(C_OBJS)
	$(CC) $(CFLAGS) -T scripts/grub.ld -o bin/$@ $(LINK_LIST)

obj/crtbegin.o obj/crtend.o:
	OBJ=`$(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=$(@F)` && cp "$$OBJ" $@

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

release:
	qemu-system-i386 -audiodev pa,id=snd0 -machine pcspk-audiodev=snd0 -fda iso/boot.iso
	# qemu-system-i386 -cdrom iso/boot.iso

debug:
	# bochs -f .bochsrc
	# qemu-system-i386 -audiodev pa,id=snd0 -machine pcspk-audiodev=snd0 -fda iso/boot.iso -s -S &
	qemu-system-i386 -cdrom iso/boot.iso -s -S &
	# gdb -x scripts/db_input.gdb
	# lldb --source scripts/lldb.in

.PHONY: GRUB clean release debug
