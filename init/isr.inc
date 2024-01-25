; asmsyntax=fasm

macro ISRErr num
{
	public ISR_#num
	ISR_#num:
		cli
		push num
		jmp ISRCommonStub
}

macro ISR num
{
	public ISR_#num
	ISR_#num:
		cli
		push 0
		push num
		jmp ISRCommonStub
}

ISR 0
ISR 1
ISR 2
ISR 3
ISR 4
ISR 5
ISR 6
ISR 7
ISRErr 8
ISR 9
ISRErr 10
ISRErr 11
ISRErr 12
ISRErr 13
ISRErr 14
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

exception_0 db "Divide By Zero", 0
exception_1 db "Debug", 0
exception_2 db "NMI", 0
exception_3 db "Breakpoint", 0
exception_4 db "Overflow", 0
exception_5 db "OOB", 0
exception_6 db "Invalid Opcode", 0
exception_7 db "No Coprocessor", 0
exception_8 db "Double Fault", 0
exception_9 db "Coprocessor Segment Overrun", 0
exception_10 db "Bad TSS", 0
exception_11 db "Segment Not Present", 0
exception_12 db "Stack Fault", 0
exception_13 db "General Protection Fault", 0
exception_14 db "Page Fault", 0
exception_15 db "Unrecognized Interrupt", 0
exception_16 db "Coprocessor Fault", 0
exception_17 db "Alignment Check", 0
exception_18 db "Machine Check", 0
exception_19 db "RESERVED", 0
exception_20 db "RESERVED", 0
exception_21 db "RESERVED", 0
exception_22 db "RESERVED", 0
exception_23 db "RESERVED", 0
exception_24 db "RESERVED", 0
exception_25 db "RESERVED", 0
exception_26 db "RESERVED", 0
exception_27 db "RESERVED", 0
exception_28 db "RESERVED", 0
exception_29 db "RESERVED", 0
exception_30 db "RESERVED", 0
exception_31 db "RESERVED", 0

extrn FaultHandler
ISRCommonStub:
	call FaultHandler
	iret

; ISRCommonStub:
; pusha
; push ds
; push es
; push fs
; push gs
; mov ax, 10h
; mov ds, ax
; mov es, ax
; mov fs, ax
; mov gs, ax
; mov eax, esp
; push eax
; mov eax, FaultHandler
; call eax
; pop eax
; pop gs
; pop fs
; pop es
; pop ds
; popa
; add esp, 8
; iret

; ISRsInstall:
; IDTSetGate 0, word [ISR_0], 0x08, 0x8E
; IDTSetGate 1, word [ISR_1], 0x08, 0x8E
; IDTSetGate 2, word [ISR_2], 0x08, 0x8E
; IDTSetGate 3, word [ISR_3], 0x08, 0x8E
; IDTSetGate 4, word [ISR_4], 0x08, 0x8E
; IDTSetGate 5, word [ISR_5], 0x08, 0x8E
; IDTSetGate 6, word [ISR_6], 0x08, 0x8E
; IDTSetGate 7, word [ISR_7], 0x08, 0x8E
; IDTSetGate 8, word [ISR_8], 0x08, 0x8E
; IDTSetGate 9, word [ISR_9], 0x08, 0x8E
; IDTSetGate 10, word [ISR_10], 0x08, 0x8E
; IDTSetGate 11, word [ISR_11], 0x08, 0x8E
; IDTSetGate 12, word [ISR_12], 0x08, 0x8E
; IDTSetGate 13, word [ISR_13], 0x08, 0x8E
; IDTSetGate 14, word [ISR_14], 0x08, 0x8E
; IDTSetGate 15, word [ISR_15], 0x08, 0x8E
; IDTSetGate 16, word [ISR_16], 0x08, 0x8E
; IDTSetGate 17, word [ISR_17], 0x08, 0x8E
; IDTSetGate 18, word [ISR_18], 0x08, 0x8E
; IDTSetGate 19, word [ISR_19], 0x08, 0x8E
; IDTSetGate 20, word [ISR_20], 0x08, 0x8E
; IDTSetGate 21, word [ISR_21], 0x08, 0x8E
; IDTSetGate 22, word [ISR_22], 0x08, 0x8E
; IDTSetGate 23, word [ISR_23], 0x08, 0x8E
; IDTSetGate 24, word [ISR_24], 0x08, 0x8E
; IDTSetGate 25, word [ISR_25], 0x08, 0x8E
; IDTSetGate 26, word [ISR_26], 0x08, 0x8E
; IDTSetGate 27, word [ISR_27], 0x08, 0x8E
; IDTSetGate 28, word [ISR_28], 0x08, 0x8E
; IDTSetGate 29, word [ISR_29], 0x08, 0x8E
; IDTSetGate 30, word [ISR_30], 0x08, 0x8E
; IDTSetGate 31, word [ISR_31], 0x08, 0x8E
