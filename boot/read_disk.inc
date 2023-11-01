; asmsyntax=fasm

macro LBAtoCHS
{
	xor bx, bx
	xor cx, cx
	xor dx, dx

	mov bx, 18
	div bx

	inc dx
	mov cl, dl

	mov bx, 2
	xor dx, dx
	div bx

	mov ch, al
	xchg dl, dh
	
	mov dl, 0 ; bsDriveNumber defined as byte so to big for 4 bits
}

macro LoadSectors size, start_lba
{
	mov ax, start_lba - 1

	.read:
		inc ax
		push ax
		push bx
		LBAtoCHS

		mov al, 01h
		mov ah, 02h
		inc cl
		pop bx
		int 13h

		add bx, 200h
		pop ax
		cmp ax, size
		jne .read
}
