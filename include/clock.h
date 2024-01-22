#pragma once

#include <stdtypes.h>
#include <interrupts.h>

#ifndef TIMER_DEF

extern u32 timer_ticks;

#endif

void TimerPhase(s32 hz);
void HandleTimer(struct Regs *r);
void InstallTimer();

void Sleep(s32 ticks);
