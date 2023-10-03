; asmsyntax=fasm
org 7c00h

KERNEL_SIZE equ 39

Start:
	xor ax, ax
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov sp, 7c00h

	call PrintStartup
	jmp A20Enable

include 'bios_fns.inc'

PrintStartup:
	mov si, start_msg
	mov ah, 0eh
	call PrintLine
	ret

A20Enable:
	call CheckA20
	cmp ax, 0
	je LoadKernel
	call BIOS_A20
	call CheckA20
	cmp ax, 0
	je LoadKernel
	call KbA20
	call CheckA20
	cmp ax, 0
	je LoadKernel
	call FastA20
	call CheckA20
	jmp LoadKernel

include 'a20.inc'

LoadKernel:
	xor ax, ax
	mov es, ax
	mov bx, 1000h

	; how many sectors to read
	mov ax, KERNEL_SIZE
	push ax

	mov dh, 0h
	mov dl, 0h
	
	mov ch, 0h
	mov cl, 02h

	.read:

		mov al, 01h
		mov ah, 02h
		inc cl

		int 13h
		add bx, 200h
		pop ax
		dec ax
		push ax
		jnz .read

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

GDT_Start:
; null
	dq 0h
; code
	dw 0ffffh
	dw 0h
	db 0h
	db 10011010b
	db 11001111b
	db 0
; data
	dw 0ffffh
	dw 0h
	db 0h
	db 10010010b
	db 11001111b
	db 0
GDT_End:

start_msg db 'Initializing!', 0

times 510 - ($ - $$) db 0
dw 0xaa55
