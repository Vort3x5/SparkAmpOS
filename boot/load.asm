 ; asmsyntax=fasm

mov sp, 1000h
 
mov si, next_msg
mov ah, 0eh
call print_line
jmp gdt_start

print_line:
	lodsb
	int 10h
	cmp al, 0
	jne print_line
; print new line
	mov al, 13
	int 10h
	mov al, 10
	int 10h
	ret

next_msg db 'cont!', 0

gdt_start:
; null
	dd 0h
	dd 0h
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

gdt:
	dw ($ - gdt_start - 1)
	dd gdt_start

protected_mode:
	cli
	lgdt [gdt]
	sti
	
	mov eax, cr0
	or eax, 1
	mov cr0, eax

	jmp 08h:jump_kernel

use32
jump_kernel:
	mov ax, 10h
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	mov esp, 090000h

	jmp 08h:2000h

times 2048 - ($ - $$) db 0
