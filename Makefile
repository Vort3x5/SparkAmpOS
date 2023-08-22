CC := i686-elf-gcc
CFLAGS := -O2 -g -nostdlib -ffreestanding -Wall -Wextra -I include/

C_SRCS := $(wildcard drivers/*.c src/*.c)
S_SRCS := $(wildcard src/*.S)

C_OBJS := $(patsubst %.c, obj/%.o, $(notdir $(C_SRCS)))
S_OBJS := $(patsubst %.S, obj/%.o, $(notdir $(S_SRCS)))

all: iso kernel_info

SparkAmpOS: $(S_OBJS) $(C_OBJS)
	$(CC) $(CFLAGS) -e _main -T scripts/link.ld -o $@ $^

obj/%.o: drivers/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

obj/%.o: src/%.S
	$(CC) $(CFLAGS) -c -o $@ $< -D__is_kernel

dirs:
	mkdir -p iso/boot/grub
	mkdir -p obj/

iso: dirs SparkAmpOS
	grub-file --is-x86-multiboot SparkAmpOS
	cp SparkAmpOS iso/boot/SparkAmpOS
	./scripts/grub.sh
	grub-mkrescue -o SparkAmpOS iso

kernel_info:
	lua scripts/file_size.lua

clean:
	rm -rf SparkAmpOS iso/ obj/

release:
	qemu-system-i386 -cdrom SparkAmpOS

debug:
	bochs -f .bochsrc
