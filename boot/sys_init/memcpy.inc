; asmsyntax=fasm

macro Memcpydw src, dest, size
{
	pusha

	; Src Buffer
	mov eax, 2000h
	mov esi, eax

	; Dest Buffer
	xor eax, eax
	mov eax, 100000h
	mov edi, eax

	; Size In Bytes
	xor ecx, ecx
	mov ecx, size

	; Move!
	rep movsd
	popa
}
