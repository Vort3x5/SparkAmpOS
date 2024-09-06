#pragma once

#include <stdtypes.h>

#ifdef CLOCK_DEF

static u32 timer_ticks = 0;

#endif

void InitTimer(s32 hz);
void HandleTimer();

void Sleep(s32 ticks);
