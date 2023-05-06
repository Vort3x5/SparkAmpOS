; asmsyntax=fasm

org 0x7c00

start:
	xor ax, ax
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov sp, 0x7c00

	call print_startup
	jmp snd_stage

print_startup:
	mov si, start_msg
	mov ah, 0x0e
	call print
	ret

print:
	lodsb
	int 0x10
	cmp al, 0
	jne print
.new_line:
	mov al, 13
	int 0x10
	mov al, 10
	int 0x10
	ret

snd_stage:
	mov bx, 0x1000
	mov es, bx
	mov bx, 0x0

	mov dh, 0x0
	mov dl, 0x0
	mov ch, 0x0
	mov cl, 0x02

	jmp read_snd

read_snd:
	mov ah, 0x02
	mov al, 0x01
	int 0x13

	jc read_snd

	mov ax, 0x1000
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	jmp 0x1000:0x0

start_msg db 'Initializing!', 0

times 510 - ($ - $$) db 0
dw 0xaa55
