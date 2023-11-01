; asmsyntax=fasm

macro IRQ num
{
	public IRQ_#num
	IRQ_#num:
		cli
		push 0
		push num + 32
		jmp IRQCommonStub
}

IRQ 0

IRQ 1

IRQ 2

IRQ 3

IRQ 4

IRQ 5

IRQ 6

IRQ 7

IRQ 8

IRQ 9

IRQ 10

IRQ 11

IRQ 12

IRQ 13

IRQ 14

IRQ 15

extrn HandleIRQ

IRQCommonStub:
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
	mov eax, HandleIRQ
	call eax
	pop eax
	pop gs
	pop fs
	pop es
	pop ds
	popa
	add esp, 8
	iret
