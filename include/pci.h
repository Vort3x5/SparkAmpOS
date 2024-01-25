#pragma once

#include <stdtypes.h>

#define CONFIG_ADDR 0xcf8
#define CONFIG_DATA 0xcfc

#define PCI_BAR0 0x10
#define PCI_BAR1 0x14
#define PCI_BAR2 0x18
#define PCI_BAR3 0x1C
#define PCI_BAR4 0x20
#define PCI_BAR5 0x24

void ScanPCI();
u32 ReadPCI(u32 bus, u32 dev, u32 function, u32 offset);
void WritePCI(u32 bus, u32 dev, u32 function, u32 offset, u32 val);
u32 PCIReadMMIOBar(u32 bus, u32 device, u32 function, u32 bar);
void PCIEnableMMIOBusMastering(u32 bus, u32 dev, u32 function);

bool IsHDA(u32 bus, u32 dev, u32 function);
void HDAFound(u32 bus, u32 dev, u32 function);
