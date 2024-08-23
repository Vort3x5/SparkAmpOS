#pragma once

#include <stdtypes.h>

static u64 ac97_base;

bool PCIIsAC97(u32 bus, u32 dev, u32 function);
void PCIAC97Found(u32 bus, u32 dev, u32 function);
