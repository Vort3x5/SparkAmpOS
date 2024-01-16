#include <clock.h>

#include <pic.h>
#include <io.h>

#define TIMER_DEF
u32 timer_ticks = 0;

void TimerPhase(s32 hz)
{
	s32 divisor = 1193180 / hz;
	OutPortB(0x43, 0x36);
	OutPortB(0x40, divisor & 0xff);
	OutPortB(0x40, divisor >> 8);
}

void HandleTimer(struct Regs *r)
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
