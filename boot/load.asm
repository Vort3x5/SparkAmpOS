 ; asmsyntax=fasm

mov sp, 1000h

mov si, next_msg
mov ah, 0eh
call print_line
jmp gdt

include 'bios_fns.inc'

next_msg db 'cont!', 0

gdt:
	dw (gdt_end - gdt_start - 1)
	dd gdt_start
 
protected_mode:
	cli
	lgdt [gdt]
	
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
	; jmp 08h:5000h

gdt_start:
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
gdt_end:

times 1024 - ($ - $$) db 0
