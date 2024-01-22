#include <interrupts.h>
#include <io.h>

void *irq_routines[16] = 
{
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0
};

void InstallIRQHandler(s32 irq, void (*handler)(struct Regs *))
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

	IDTSetGate(32, (dword)IRQ_0, 0x08, 0x8e);
	IDTSetGate(33, (dword)IRQ_1, 0x08, 0x8e);
	IDTSetGate(34, (dword)IRQ_2, 0x08, 0x8e);
	IDTSetGate(35, (dword)IRQ_3, 0x08, 0x8e);
	IDTSetGate(36, (dword)IRQ_4, 0x08, 0x8e);
	IDTSetGate(37, (dword)IRQ_5, 0x08, 0x8e);
	IDTSetGate(38, (dword)IRQ_6, 0x08, 0x8e);
	IDTSetGate(39, (dword)IRQ_7, 0x08, 0x8e);
	IDTSetGate(40, (dword)IRQ_8, 0x08, 0x8e);
	IDTSetGate(41, (dword)IRQ_9, 0x08, 0x8e);
	IDTSetGate(42, (dword)IRQ_10, 0x08, 0x8e);
	IDTSetGate(43, (dword)IRQ_11, 0x08, 0x8e);
	IDTSetGate(44, (dword)IRQ_12, 0x08, 0x8e);
	IDTSetGate(45, (dword)IRQ_13, 0x08, 0x8e);
	IDTSetGate(46, (dword)IRQ_14, 0x08, 0x8e);
	IDTSetGate(47, (dword)IRQ_15, 0x08, 0x8e);
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
