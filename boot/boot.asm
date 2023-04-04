; asmsyntax=fasm

org 0x7c00

use16

.start:	
	call .enable_a20
	jmp .loop

.loop:
	jmp $

.enable_a20:
	call .check_a20
	call .a20_bios_enable
	call .check_a20
	call .a20_kb_enable
	call .check_a20
	call .a20_fast_enable
	call .check_a20
	ret

.check_a20:
	call .check_a20_proc
	popf
	cmp ax, 1
	je .start
	ret

.check_a20_proc:
	pushf
	push ds
	push es
	push di
	push si

	cli
	xor ax, ax
	mov es, ax
	not ax
	mov ds, ax
	mov di, 0x0500
	mov si, 0x0510
	mov al, byte [es:di]
	push ax
	mov al, byte [ds:si]
	push ax
	mov byte [es:di], 0x00
	mov byte [ds:si], 0xFF
	cmp byte [es:di], 0xFF
	pop ax
	mov byte [ds:si], al
	pop ax
	mov byte [es:di], al
	mov ax, 0

	pop si
	pop di
	pop es
	pop ds
	je .enable_a20
	mov ax, 1
	ret

.a20_enabled:
	xor ax, ax
	xor bx, bx
	jmp .start

.a20_bios_enable:
	mov ax, 0x2401
	int 0x15
	ret

.a20_kb_enable:
	cli
	mov al, 0xd1
	out 0x64, al
	call .delay
	mov al, 0xdf
	in al, 0x60
	sti
	ret

.a20_fast_enable:
	in al, 0x92
	or al, 2
	out 0x92, al
	ret

.delay:
	push cx
	mov cx, 0xfffe
	.delay_loop:
		loop .delay_loop
	pop cx
	ret


times 510 - ($ - .start) db 0
dw 0xaa55
