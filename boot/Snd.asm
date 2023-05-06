; asmsyntax=fasm

gdt_start equ 0x800
setup_gdt:
	.gdt_null:
		dw 0
		dw 0
	.gdt_code:
		dw 0xffff
		dw 0
		db 0
		db 0x9a
		db 0xcf
		db 0x00
	.gdt_data:
		dw 0xffff
		dw 0
		db 0
		db 0x92
		db 0xcf
		db 0

gdt:
	dw gdt_end - gdt_start - 1
	dd gdt_start
		
gdt_end:

load_gdt:
	cli
	lgdt [gdt]
	sti

a20_enable:
	jmp check_a20
	cmp ax, 0
	jne bios_a20
	jmp check_a20
	cmp ax, 0
	jne kb_a20
	jmp check_a20
	cmp ax, 0
	jne fast_a20
	jmp check_a20
	jmp protected_mode

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
	jmp a20_enable

bios_a20:
	mov ax, 0x2401
	int 0x15
	jmp a20_enable

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
	jmp a20_enable

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
	jmp a20_enable

protected_mode:
	mov eax, cr0
	or eax, 1
	mov cr0, eax
	jmp regset
	nop
	nop

regset:
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	mov esp, 0x2ffff

times 512 - ($ - $$) db 0
