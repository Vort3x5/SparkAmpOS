#pragma once

#include <stdtypes.h>
#include <interrupts.h>

void TimerPhase(s32 hz);
void HandleTimer();
void InstallTimer();

void Sleep(s32 ticks);
