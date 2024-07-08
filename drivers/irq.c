#include <interrupts.h>
#include <io.h>

void *irq_routines[16] = 
{
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0
};

void InstallIRQHandler(s32 irq, void *handler)
{
	irq_routines[irq] = handler;
}

void UninstallIRQHandler(s32 irq)
{
	irq_routines[irq] = 0;
}

void RemapIRQ(void)
{
	OutB(0x20, 0x11);
	OutB(0xA0, 0x11);
	OutB(0x21, 0x20);
	OutB(0xA1, 0x28);
	OutB(0x21, 0x04);
	OutB(0xA1, 0x02);
	OutB(0x21, 0x01);
	OutB(0xA1, 0x01);
	OutB(0x21, 0x0);
	OutB(0xA1, 0x0);
}

void IRQsInstall()
{
	RemapIRQ();

	IDTSetGate(32, (dword)IRQ_0);
	IDTSetGate(33, (dword)IRQ_1);
	IDTSetGate(34, (dword)IRQ_2);
	IDTSetGate(35, (dword)IRQ_3);
	IDTSetGate(36, (dword)IRQ_4);
	IDTSetGate(37, (dword)IRQ_5);
	IDTSetGate(38, (dword)IRQ_6);
	IDTSetGate(39, (dword)IRQ_7);
	IDTSetGate(40, (dword)IRQ_8);
	IDTSetGate(41, (dword)IRQ_9);
	IDTSetGate(42, (dword)IRQ_10);
	IDTSetGate(43, (dword)IRQ_11);
	IDTSetGate(44, (dword)IRQ_12);
	IDTSetGate(45, (dword)IRQ_13);
	IDTSetGate(46, (dword)IRQ_14);
	IDTSetGate(47, (dword)IRQ_15);
}

void HandleIRQ(struct Regs *r)
{
	void (*handler) (struct Regs *r);

	handler = irq_routines[r->int_no - 32];
	if (handler)
		handler(r);

	if (r->int_no >= 40)
		OutB(0xA0, 0x20);

	OutB(0x20, 0x20);
}
