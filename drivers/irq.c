#include <irq.h>

#include <stdtypes.h>
#include <memory.h>
#include <video.h>

struct IDT_Entry idt[256];
struct IDT_Ptr idtp;

void IDTLoad()
{
	asm volatile (
		"lidt [idtp];"
	);
}

void IDTSetGate(byte index, dword base, word sel, byte flags)
{
	idt[index] = (struct IDT_Entry) {
		.base_lo = (idtp.base) & 0xffff,
		.base_hi = (idtp.base >> 16) & 0xffff,
		.sel = sel,
		.flags = flags | 0x60,
		.always0 = 0
	};
}

void IDTInstall()
{
	idtp.limit = (sizeof (struct IDT_Entry) * 256) - 1;
	idtp.base = (dword) &idt[0];

	Memset(&idt, 0, sizeof (struct IDT_Entry) * 256);

	IDTLoad();
}

void ISRsInstall()
{
	IDTSetGate(0, (dword) ISR_0, 0x08, 0x8E);
	IDTSetGate(0, (dword) ISR_1, 0x08, 0x8E);
	IDTSetGate(0, (dword) ISR_2, 0x08, 0x8E);
	IDTSetGate(0, (dword) ISR_3, 0x08, 0x8E);
	IDTSetGate(0, (dword) ISR_4, 0x08, 0x8E);
	IDTSetGate(0, (dword) ISR_5, 0x08, 0x8E);
	IDTSetGate(0, (dword) ISR_6, 0x08, 0x8E);
	IDTSetGate(0, (dword) ISR_7, 0x08, 0x8E);
	IDTSetGate(0, (dword) ISR_8, 0x08, 0x8E);
	IDTSetGate(0, (dword) ISR_9, 0x08, 0x8E);
	IDTSetGate(0, (dword) ISR_10, 0x08, 0x8E);
	IDTSetGate(0, (dword) ISR_11, 0x08, 0x8E);
	IDTSetGate(0, (dword) ISR_12, 0x08, 0x8E);
	IDTSetGate(0, (dword) ISR_13, 0x08, 0x8E);
	IDTSetGate(0, (dword) ISR_14, 0x08, 0x8E);
	IDTSetGate(0, (dword) ISR_15, 0x08, 0x8E);
	IDTSetGate(0, (dword) ISR_16, 0x08, 0x8E);
	IDTSetGate(0, (dword) ISR_17, 0x08, 0x8E);
	IDTSetGate(0, (dword) ISR_18, 0x08, 0x8E);
	IDTSetGate(0, (dword) ISR_19, 0x08, 0x8E);
	IDTSetGate(0, (dword) ISR_20, 0x08, 0x8E);
	IDTSetGate(0, (dword) ISR_21, 0x08, 0x8E);
	IDTSetGate(0, (dword) ISR_22, 0x08, 0x8E);
	IDTSetGate(0, (dword) ISR_23, 0x08, 0x8E);
	IDTSetGate(0, (dword) ISR_24, 0x08, 0x8E);
	IDTSetGate(0, (dword) ISR_25, 0x08, 0x8E);
	IDTSetGate(0, (dword) ISR_26, 0x08, 0x8E);
	IDTSetGate(0, (dword) ISR_27, 0x08, 0x8E);
	IDTSetGate(0, (dword) ISR_28, 0x08, 0x8E);
	IDTSetGate(0, (dword) ISR_29, 0x08, 0x8E);
	IDTSetGate(0, (dword) ISR_30, 0x08, 0x8E);
	IDTSetGate(0, (dword) ISR_31, 0x08, 0x8E);
}

char *exceptions[] = 
{
    "Divide By Zero",
    "Debug",
    "NMI",
    "Breakpoint",
    "Overflow",
    "OOB",
    "Invalid Opcode",
    "No Coprocessor",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unrecognized Interrupt",
    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "RESERVED",
    "RESERVED",
    "RESERVED",
    "RESERVED",
    "RESERVED",
    "RESERVED",
    "RESERVED",
    "RESERVED",
    "RESERVED",
    "RESERVED",
    "RESERVED"
};

void FaultHandler(struct Regs *reg, TTY *tty)
{
	if (reg -> int_no < 32)
	{
		Print(tty, exceptions[reg -> int_no], RED);
		Print(tty, " Exception. System Halted!\n", RED);
		for(;;);
	}
}
