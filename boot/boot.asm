; asmsyntax=fasm
org 7c00h

; SND_SIZE equ 4
KERNEL_SIZE equ 60

Start:
	xor ax, ax
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov sp, 7c00h

	call PrintStartup
	jmp A20Enable

include 'print.inc'

PrintStartup:
	mov si, start_msg
	mov ah, 0eh
	call PrintLine
	ret

A20Enable:
	call CheckA20 ; returns 0 if a20 is disabled and 1 if enabled
	cmp ax, 1
	je LoadKernel
	call BIOS_A20
	call CheckA20
	cmp ax, 1
	je LoadKernel
	call KbA20
	call CheckA20
	cmp ax, 1
	je LoadKernel
	call FastA20
	call CheckA20
	jmp LoadKernel

include 'a20.inc'

include 'read_disk.inc'

LoadKernel:
	xor ax, ax
	mov es, ax
	mov bx, 1000h

	LoadSectors KERNEL_SIZE, 1 ; 1 + second_stage size in sectors

GDT:
	dw (GDT_End - GDT_Start - 1)
	dd GDT_Start
 
ProtectedMode:
	cli
	lgdt [GDT]
	
	mov eax, cr0
	or eax, 1
	mov cr0, eax

	jmp 08h:JumpKernel

use32
JumpKernel:
	mov ax, 10h
	mov ds, ax
	mov fs, ax

	mov gs, ax
	mov es, ax
	mov ss, ax
	mov esp, 09000h
	mov ebp, esp

	jmp 08h:1000h

include 'gdt.inc'

start_msg db 'Initializing!', 0

times 510 - ($ - $$) db 0
dw 0xaa55
