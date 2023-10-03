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

section '.bss' align 16

stack_bottom dd 0
stack_top equ stack_bottom + 16384

section '.text'

extrn Main
public _Start
_Start:
	mov esp, stack_top

	call Main
	
	cli
	hlt

extrn idtp
public _IDTLoad
_IDTLoad:
	lidt [idtp]
	ret

include 'isr.inc'

extrn FaultHandler

ISRCommonStub:
	pusha
	push ds
	push es
	push fs
	push gs
	mov ax, 10h
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov eax, esp
	push eax
	mov eax, FaultHandler
	call eax
	pop eax
	pop gs
	pop fs
	pop es
	pop ds
	popa
	add esp, 8
	iret
