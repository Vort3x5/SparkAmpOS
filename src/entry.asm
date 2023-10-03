; asmsyntax=fasm

format ELF

ALN			equ 1<<0 ; ALIGN is a keyword in fasm
MEMINFO		equ 1<<1
FLAGS 		equ 1 or 2 ; 1<<0 | 1<<1
MAGIC 		equ 0x1badb002
CHECKSUM 	equ -(MAGIC + FLAGS)

section '.multiboot' align 4
multiboot:
	dd MAGIC
	dd 1 or 2
	dd CHECKSUM

section '.bss' align 16

stack_bottom dd 0
stack_top equ stack_bottom + 16384

section '.text'

extrn Main
public _start
_start:
	mov esp, stack_top

	call Main
	
	cli

	hlt
