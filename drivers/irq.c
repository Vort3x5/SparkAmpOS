#include <pic.h>
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
	OutPortB(0x20, 0x11);
	OutPortB(0xA0, 0x11);
	OutPortB(0x21, 0x20);
	OutPortB(0xA1, 0x28);
	OutPortB(0x21, 0x04);
	OutPortB(0xA1, 0x02);
	OutPortB(0x21, 0x01);
	OutPortB(0xA1, 0x01);
	OutPortB(0x21, 0x0);
	OutPortB(0xA1, 0x0);
}

void InstallIRQ()
{
	RemapIRQ();

	IDTSetGate(32, (unsigned)IRQ_0, 0x08, 0x8e);
	IDTSetGate(33, (unsigned)IRQ_1, 0x08, 0x8e);
	IDTSetGate(34, (unsigned)IRQ_2, 0x08, 0x8e);
	IDTSetGate(35, (unsigned)IRQ_3, 0x08, 0x8e);
	IDTSetGate(36, (unsigned)IRQ_4, 0x08, 0x8e);
	IDTSetGate(37, (unsigned)IRQ_5, 0x08, 0x8e);
	IDTSetGate(38, (unsigned)IRQ_6, 0x08, 0x8e);
	IDTSetGate(39, (unsigned)IRQ_7, 0x08, 0x8e);
	IDTSetGate(40, (unsigned)IRQ_8, 0x08, 0x8e);
	IDTSetGate(41, (unsigned)IRQ_9, 0x08, 0x8e);
	IDTSetGate(42, (unsigned)IRQ_10, 0x08, 0x8e);
	IDTSetGate(43, (unsigned)IRQ_11, 0x08, 0x8e);
	IDTSetGate(44, (unsigned)IRQ_12, 0x08, 0x8e);
	IDTSetGate(45, (unsigned)IRQ_13, 0x08, 0x8e);
	IDTSetGate(46, (unsigned)IRQ_14, 0x08, 0x8e);
	IDTSetGate(47, (unsigned)IRQ_15, 0x08, 0x8e);
}

void HandleIRQ(struct Regs *r)
{
	void (*handler) (struct Regs *r);

	handler = irq_routines[r->int_no - 32];
	if (handler)
		handler(r);

	if (r->int_no >= 40)
		OutPortB(0xA0, 0x20);

	OutPortB(0x20, 0x20);
}
