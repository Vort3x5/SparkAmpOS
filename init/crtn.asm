; asmsyntax=fasm

format ELF

section '.init'
	pop ebp
	ret

section '.fini'
	pop ebp
	ret
