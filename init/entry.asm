; asmsyntax=fasm

format ELF

ALN			equ 1<<0 ; ALIGN is a keyword in fasm
MEMINFO		equ 1<<1
FLAGS 		equ 1 or 2 ; 1<<0 | 1<<1
MAGIC 		equ 0x1badb002
CHECKSUM 	equ -(MAGIC + FLAGS)

section '.multiboot' align 4
Multiboot:
	dd MAGIC
	dd 1 or 2
	dd CHECKSUM

section '.text' align 4

extrn Main
public _Start
_Start:
	mov esp, stack_top
	cld

	call Main
	
	cli
halt: 	
	hlt
	jmp halt

extrn idtp
public _IDTLoad
_IDTLoad:
	lidt [idtp]
	ret

public _FPUInit
_FPUInit:
    clts
	fninit

	mov eax, cr0
	and eax, 0xFFFFFFF3
	or eax, 0x00000002
	mov cr0, eax

	mov eax, cr4
	or eax, 0x00000600
	mov cr4, eax

	ret

public _Halt
_Halt:
	cli
	hlt

include 'isr.inc'

include 'irq.inc'

section '.bss' align 4

stack_bottom:
	rb 4096
stack_top:
