#define IRQ_DEF

#include <interrupts.h>
#include <io.h>
#include <video.h>

void *irq_routines[16] = 
{
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0
};

void InstallIRQHandler(void *handler, s32 irq_slot)
{
	irq_routines[irq_slot] = handler;
}

void UninstallIRQHandler(s32 irq_slot)
{
	irq_routines[irq_slot] = 0;
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
	OutB(0x21, 0xFB);
	OutB(0xA1, 0xFF);
}

void IRQsInstall()
{
	RemapIRQ();

	IDTSetGate(32, (u32)IRQ_0);
	IDTSetGate(33, (u32)IRQ_1);
	IDTSetGate(34, (u32)IRQ_2);
	IDTSetGate(35, (u32)IRQ_3);
	IDTSetGate(36, (u32)IRQ_4);
	IDTSetGate(37, (u32)IRQ_5);
	IDTSetGate(38, (u32)IRQ_6);
	IDTSetGate(39, (u32)IRQ_7);
	IDTSetGate(40, (u32)IRQ_8);
	IDTSetGate(41, (u32)IRQ_9);
	IDTSetGate(42, (u32)IRQ_10);
	IDTSetGate(43, (u32)IRQ_11);
	IDTSetGate(44, (u32)IRQ_12);
	IDTSetGate(45, (u32)IRQ_13);
	IDTSetGate(46, (u32)IRQ_14);
	IDTSetGate(47, (u32)IRQ_15);
}

void HandleIRQ(s32 int_num)
{
	void (*handler) (s32 int_num);

	handler = irq_routines[int_num - 32];
	if (handler)
		handler(int_num);

	if (int_num >= 40)
		OutB(0xA0, 0x20);

	OutB(0x20, 0x20);
}
