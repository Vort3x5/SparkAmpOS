; asmsyntax=fasm

struc MemMapEntry 
{
	.base_addr dq ?
	.len dq ?
	.type dd ?
	.EAB dd ? ; ACPI 3.0 Extended Attributes bitfield (if 24 bytes are returned, instead of 20) 
}

virtual at 0
	MemMapEntry MemMapEntry
	sizeof.MemMapEntry = $ - MemMapEntry
end virtual

mmap_entry_addr equ 4000h
E820:
	mov di, 3004h
	xor ebx, ebx
	xor bp, bp
	mov edx, 0534d4150h
	mov eax, 0e820h
	mov [es:di + 20], dword 1
	mov ecx, 24
	int 15h
	jc .failed
	mov edx, 0534d4150h
	cmp eax, edx
	jne short .failed
	test ebx, ebx
	je short .failed
	jmp short .jmpin

.E8201p:
	mov eax, 0e820h
	mov [es:di + 20], dword 1
	mov ecx, 24
	int 15h
	jc short .E820f
	mov edx, 0534d4150h

.jmpin:
	jcxz .skipent	; skip 0 len entries
	cmp cl, 20		; check how many bytes is one entry
	jbe short .notext
	test byte [es:di + 20], 1
	je short .skipent

.notext:
	mov ecx, [es:di + 8]
	or ecx, [es:di + 12]
	jz .skipent
	inc bp
	add di, 24

.skipent:
	test ebx, ebx
	jne short .E8201p 

.E820f:
	mov [mmap_entry_addr], bp
	clc
	ret

.failed:
	stc
	ret
