; asmsyntax=fasm

use16
org 7c00h

jmp Start

include '../sys_init/OEMpb.inc'

SND_SIZE equ 4
include '../sys_init/kernel_size.inc'

include '../bios_funcs/print.inc'

Start:
	xor ax, ax

	mov ds, ax
	mov fs, ax
	mov gs, ax
	mov es, ax

	mov sp, 7c00h

	cld

	Print start_msg

	jmp LoadSND

include '../bios_funcs/read_disk.inc'

LoadSND:
	xor ax, ax
	mov es, ax
	mov bx, 1000h

	LoadSectors SND_SIZE, 0

LoadKernel:
	xor ax, ax
	mov es, ax
	mov bx, 7e00h

	LoadSectors KERNEL_SIZE, SND_SIZE

JumpSND:
	xor ax, ax

	mov ds, ax
	mov fs, ax
	mov gs, ax
	mov es, ax
	mov ss, ax

	jmp 0h:1000h

start_msg db 'Initializing!', 0

times 510 - ($ - $$) db 0
dw 0xaa55
