#pragma once

#include <stdtypes.h>
#include <interrupts.h>

#ifndef TTY_DEF

extern u32 timer_ticks;

#endif

void InitTimer(s32 hz);
void HandleTimer();

void Sleep(s32 ticks);
