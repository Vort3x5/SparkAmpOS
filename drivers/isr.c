#include <interrupts.h>

#include <stdtypes.h>
#include <video.h>

void ISRsInstall()
{
	IDTSetGate(0, (u32) ISR_0);
	IDTSetGate(1, (u32) ISR_1);
	IDTSetGate(2, (u32) ISR_2);
	IDTSetGate(3, (u32) ISR_3);
	IDTSetGate(4, (u32) ISR_4);
	IDTSetGate(5, (u32) ISR_5);
	IDTSetGate(6, (u32) ISR_6);
	IDTSetGate(7, (u32) ISR_7);
	IDTSetGate(8, (u32) ISR_8);
	IDTSetGate(9, (u32) ISR_9);
	IDTSetGate(10, (u32) ISR_10);
	IDTSetGate(11, (u32) ISR_11);
	IDTSetGate(12, (u32) ISR_12);
	IDTSetGate(13, (u32) ISR_13);
	IDTSetGate(14, (u32) ISR_14);
	IDTSetGate(15, (u32) ISR_15);
	IDTSetGate(16, (u32) ISR_16);
	IDTSetGate(17, (u32) ISR_17);
	IDTSetGate(18, (u32) ISR_18);
	IDTSetGate(19, (u32) ISR_19);
	IDTSetGate(20, (u32) ISR_20);
	IDTSetGate(21, (u32) ISR_21);
	IDTSetGate(22, (u32) ISR_22);
	IDTSetGate(23, (u32) ISR_23);
	IDTSetGate(24, (u32) ISR_24);
	IDTSetGate(25, (u32) ISR_25);
	IDTSetGate(26, (u32) ISR_26);
	IDTSetGate(27, (u32) ISR_27);
	IDTSetGate(28, (u32) ISR_28);
	IDTSetGate(29, (u32) ISR_29);
 	IDTSetGate(30, (u32) ISR_30);
	IDTSetGate(31, (u32) ISR_31);
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
    "RESERVED",
    "RESERVED",
    "RESERVED"
};

void FaultHandler(s32 int_num)
{
	if (int_num < 32)
	{
		Print(exceptions[int_num], RED);
		Print(" Exception. System Halted!\n", RED);
	}
	else
		Print("Exception Out Of ISR Range. System Halted!\n", RED);
	_Halt();
}
