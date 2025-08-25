#pragma once

#include <stdtypes.h>

u8 In8(u16 port);
u16 In16(u16 port);
u32 In32(u16 port);
void Out8(u16 port, u8 data);
void Out16(u16 port, u16 data);
void Out32(u16 port, u32 data);

u8 MMIn8(u32 addr);
u16 MMIn16(u32 addr);
u32 MMIn32(u32 addr);
void MMOut8(u32 addr, u8 val);
void MMOut16(u32 addr, u16 val);
void MMOut32(u32 addr, u32 val);
