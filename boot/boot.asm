; asmsyntax=fasm

org 0x7c00

use16

.regset: ; Setup 16-bit segment registers and stack
	push ax

	xor ax, ax
	mov ds, ax
	mov es, ax

	mov ss, ax
	mov sp, 0x7c00

	pop ax

.start_msg db 'Initializing!', 0

.start:	
	call .print_startup
	jmp $

.print_startup:
	push si
	mov si, .start_msg
	mov ah, 0x0e
	call .print
	ret

.print:
	lodsb
	int 0x10
	cmp al, 0
	jne .print
	ret

times 510 - ($ - .regset) db 0
dw 0xaa55
