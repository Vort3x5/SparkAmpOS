#pragma once

#include <stdtypes.h>

u8 InB(u16 port);
u16 InW(u16 port);
u32 InL(u32 port);

void OutB(u16 port, u8 data);
void OutW(u16 port, u16 data);
void OutL(u16 port, u32 data);
