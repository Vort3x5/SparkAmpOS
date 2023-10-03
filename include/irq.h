#pragma once

#include <stdtypes.h>
#include <video.h>

struct IDT_Entry
{
	word base_lo;
	word sel;
	byte always0;
	byte flags;
	word base_hi;
} __attribute__((packed));

struct IDT_Ptr
{
	word limit;
	dword base;
} __attribute__((packed));

void IDTLoad();
void IDTSetGate(byte index, dword base, word sel, byte flags);
void IDTInstall();

struct Regs
{
	u32 gs, fs, es, ds;
	u32 edi, esi, ebp, esp, ebx, edx, ecx, eax;
	u32 int_no, err_code;
	u32 eip, cs, eflags, useresp, ss;
};

extern void ISR_0(struct Regs *);
extern void ISR_1(struct Regs *);
extern void ISR_2(struct Regs *);
extern void ISR_3(struct Regs *);
extern void ISR_4(struct Regs *);
extern void ISR_5(struct Regs *);
extern void ISR_6(struct Regs *);
extern void ISR_7(struct Regs *);
extern void ISR_8(struct Regs *);
extern void ISR_9(struct Regs *);
extern void ISR_10(struct Regs *);
extern void ISR_11(struct Regs *);
extern void ISR_12(struct Regs *);
extern void ISR_13(struct Regs *);
extern void ISR_14(struct Regs *);
extern void ISR_15(struct Regs *);
extern void ISR_16(struct Regs *);
extern void ISR_17(struct Regs *);
extern void ISR_18(struct Regs *);
extern void ISR_19(struct Regs *);
extern void ISR_20(struct Regs *);
extern void ISR_21(struct Regs *);
extern void ISR_22(struct Regs *);
extern void ISR_23(struct Regs *);
extern void ISR_24(struct Regs *);
extern void ISR_25(struct Regs *);
extern void ISR_26(struct Regs *);
extern void ISR_27(struct Regs *);
extern void ISR_28(struct Regs *);
extern void ISR_29(struct Regs *);
extern void ISR_30(struct Regs *);
extern void ISR_31(struct Regs *);

void ISRsInstall();
void FaultHandler(struct Regs *reg, TTY *tty);
