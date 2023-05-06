; asmsyntax=fasm

print_loaded:
	mov si, loaded_msg
	mov ah, 0x0e

print:
	lodsb
	int 0x10
	cmp al, 0
	jne print

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

cli
lgdt [gdt]
sti

halt:
	cli
	hlt

loaded_msg db 'Loaded!', 0

times 512 - ($ - $$) db 0
