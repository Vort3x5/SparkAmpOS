; asmsyntax=fasm

use32
	mov edi, 0xB8000    ; Pointer to video memory (0xB8000)
	mov al, 'P'
	mov ah, 0fh
	stosw

times 512 - ($ - $$) db 0
