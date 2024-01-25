#define TIMER_DEF
#include <clock.h>

#include <interrupts.h>
#include <io.h>

u32 timer_ticks = 0;

void TimerPhase(s32 hz)
{
	s32 divisor = 1193180 / hz;
	OutB(0x43, 0x36);
	OutB(0x40, divisor & 0xff);
	OutB(0x40, divisor >> 8);
}

// void HandleTimer(struct Regs *r)
void HandleTimer()
{
	++timer_ticks;
}

void InstallTimer()
{
	InstallIRQHandler(0, HandleTimer);
}

void Sleep(s32 ticks)
{
	u32 eticks = timer_ticks + ticks;

	while (timer_ticks < eticks);
}
