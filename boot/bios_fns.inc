; asmsyntax=fasm

print:
	lodsb
	int 10h
	cmp al, 0
	jne print
	ret

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

disk_read:
	mov ah, 02h
	int 13h

	jc disk_read
	ret
