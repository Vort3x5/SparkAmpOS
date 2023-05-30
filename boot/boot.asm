 ; asmsyntax=fasm
org 7c00h

regset:
	xor ax, ax
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov sp, 0x7c00

	call print_startup
	jmp a20_enable

include 'bios_fns.inc'

print_startup:
	mov si, start_msg
	mov ah, 0x0e
	call print_line
	ret

a20_enable:
	call check_a20
	cmp ax, 0
	je load_snd
	call bios_a20
	call check_a20
	cmp ax, 0
	je load_snd
	call kb_a20
	call check_a20
	cmp ax, 0
	je load_snd
	call fast_a20
	call check_a20
	jmp load_snd

check_a20:
	pushf
	push ds
	push es
	push di
	push si

	cli
	xor ax, ax
	mov es, ax
	mov di, 0x0500
	mov ax, 0xffff
	mov ds, ax
	mov si, 0x0510
	mov al, byte [es:di]
	push ax
	mov al, byte [ds:si]
	push ax
	mov byte [es:di], 0x00
	mov byte [ds:si], 0xff
	cmp byte [es:di], 0xff
	pop ax
	mov byte [ds:si], al
	pop ax
	mov byte [es:di], al
	
	mov ax, 0
	je end_check
	mov ax, 1

end_check:
	pop si
	pop di
	pop es
	pop ds
	popf
	ret

bios_a20:
	mov ax, 0x2401
	int 0x15
	ret

kb_a20:
	cli

	call a20_wait_command
	mov al, 0xad
	out 0x64, al

	call a20_wait_command
	mov al, 0xd0
	out 0x64, al

	call a20_wait_data
	in al, 0x60
	push eax

	call a20_wait_command
	mov al, 0xd1
	out 0x64, al

	call a20_wait_command
	pop eax
	or al, 2
	out 0x60, al

	call a20_wait_command
	mov al, 0xae
	out 0x64, al

	call a20_wait_command

	sti
	ret

a20_wait_command:
	in al, 0x64
	test al, 2
	jnz a20_wait_command
	ret

a20_wait_data:
	in al, 0x64
	test al, 1
	jz a20_wait_data
	ret

fast_a20:
	in al, 0x92
	or al, 2
	out 0x92, al
	ret

load_snd:
	mov bx, 1000h
	mov es, bx
	mov bx, 0h

	mov dh, 0h
	mov dl, 0h
	mov ch, 0h
	mov cl, 02h

	mov al, 04h

	call disk_read

	mov ax, 1000h
	mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    jmp 1000h:0h

start_msg db 'Initializing!', 0

times 510 - ($ - $$) db 0
dw 0xaa55
