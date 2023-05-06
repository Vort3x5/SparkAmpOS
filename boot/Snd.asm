; asmsyntax=fasm

print_loaded:
	mov si, loaded_msg
	mov ah, 0x0e
	call print
	jmp halt

print:
	lodsb
	int 0x10
	cmp al, 0
	jne print
	ret

halt:
	cli
	hlt

loaded_msg db 'Loaded!', 0

times 512 - ($ - $$) db 0
