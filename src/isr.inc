; asmsyntax=fasm

macro ISR num
{
	public ISR_#num
	ISR_#num:
		cli
		push num
		jmp ISRCommonStub
}

macro ISRErr num
{
	public ISR_#num
	ISR_#num:
		cli
		push 0
		push num
		jmp ISRCommonStub
}

ISRErr 0

ISR 1

ISR 2

ISR 3

ISR 4

ISR 5

ISR 6

ISR 7

ISR 8

ISR 9

ISR 10

ISR 11

ISR 12

ISR 13

ISR 14

ISR 15

ISR 16

ISR 17

ISR 18

ISR 19

ISR 20

ISR 21

ISR 22

ISR 23

ISR 24

ISR 25

ISR 26

ISR 27

ISR 28

ISR 29

ISR 30

ISR 31

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
