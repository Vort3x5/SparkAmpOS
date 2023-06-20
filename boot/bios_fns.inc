; asmsyntax=fasm

Print:
	lodsb
	int 10h
	cmp al, 0
	jne Print
	ret

PrintLine:
	lodsb
	int 10h
	cmp al, 0
	jne PrintLine
	; print new line
	mov al, 13
	int 10h
	mov al, 10
	int 10h
	ret
