#pragma once

#include <stdtypes.h>
#include <pic.h>

extern u32 timer_ticks;

void TimerPhase(s32 hz);
void HandleTimer(struct Regs *r);
void InstallTimer();

void Sleep(s32 ticks);
