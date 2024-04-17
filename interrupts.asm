format elf

struc IDT_Entry
{
	.base_lo  dw ?
	.selector dw ?
	.always0  db ?
	.flags    db ?
	.base_hi  dw ?
}
virtual at 0
	IDT_Entry IDT_Entry
	idt_entry_size = $ 
end virtual

struc IDT_Ptr
{
	.limit dw ?
	.base  dd ?
}
virtual at 0
	IDT_Ptr IDT_Ptr
	idt_ptr_size = $ 
end virtual

idt IDT_Entry 
rb (idt_entry_size * 256)

idt_ptr IDT_Ptr 
rb idt_ptr_size

extrn Memset
public _IDTLoad
_IDTLoad:
	mov [idt_ptr + IDT_Ptr.limit], (idt_entry_size * 256) - 1
	mov [idt_ptr + IDT_Ptr.base], idt

	push (idt_entry_size * 256) - 1
	push 0
	push idt
	call Memset
	add esp, 0x10

	lidt [idt_ptr]

macro IDTSetGate index, base, selector, flags
{
	push eax
	xor ax, ax
	mov ax, base
	and ax, 0xffff
	mov [idt + (index * idt_entry_size) + IDT_Entry.base_lo], ax
	xor ax, ax
	mov ax, base
	shr ax, 16
	and ax, 0xffff
	mov [idt + (index * idt_entry_size) + IDT_Entry.base_hi], ax
	mov [idt + (index * idt_entry_size) + IDT_Entry.selector], selector
	xor al, al
	mov al, flags
	or al, 0x60
	mov [idt + (index * idt_entry_size) + IDT_Entry.flags], al
	mov [idt + (index * idt_entry_size) + IDT_Entry.always0], 0
	pop eax
}

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

include 'init/isr.inc'

ISRCommonStub:
	call FaultHandler
	iret

extrn Print
FaultHandler:
	pop ax
	cmp ax, 31
	jbe .msg
	hlt
	.msg:
		; pass the color to print the message in
		push 4
		PassMsg word [ax]

struc IRQ_Handlers
{ .handler dq ? }
virtual at 0
	IRQ_Handlers IRQ_Handlers
	irq_handlers_size = $ 
end virtual

irq_handlers IRQ_Handlers
rb (irq_handlers_size * 16)

macro InstallIRQHandler irq, handler
{ mov [irq_handlers + (irq * irq_handlers_size) + IRQ_Handlers.handler], handler }

macro UninstallIRQHandler irq
{ mov [irq_handlers + (irq * irq_handlers_size) + IRQ_Handlers.handler], 0 }
