#include <pci.h>

#include <stdtypes.h>
#include <io.h>
#include <video.h>
#include <interrupts.h>
#include <hda.h>

void ScanPCI()
{
	hda_sc_ptr = 0;
	for (s32 bus = 0; bus < 256; ++bus)
	{
		for (s32 dev = 0; dev < 32; ++dev)
		{
			if (IsHDA(bus, dev, 0))
			{
				HDAFound(bus, dev, 0);
				return;
			}

			if ((ReadPCI(bus, dev, 0, 0x0C) & 0x00800000) == 0x00800000)
			{
				for (s32 function = 1; function < 8; ++function)
				{
					if (IsHDA(bus, dev, function))
					{
						HDAFound(bus, dev, 0);
						return;
					}
				}
			}
		}
	}
	Print("Fatal: HDA not found", RED);
	_Halt();
}

void WritePCI(u32 bus, u32 dev, u32 function, u32 offset, u32 val)
{
	OutL(CONFIG_ADDR, (0x80000000 | (bus << 16) | (dev << 11) | (function << 8) | (offset)));
	OutL(CONFIG_DATA, val);
}

u32 PCIReadMMIOBar(u32 bus, u32 dev, u32 function, u32 bar)
{
	OutL(CONFIG_ADDR, 
			(0x80000000 
			 | (bus << 16) 
			 | (dev << 11) 
			 | (function << 8) 
			 | (bar)));
	return (InL(CONFIG_DATA) & 0xfffffff0);
}

void PCIEnableMMIOBusMastering(u32 bus, u32 dev, u32 function)
{
	WritePCI(bus, dev, function, 0x04, 
			((ReadPCI(bus, dev, function, 0x04) 
			  & ~(1 << 10)) | (1 << 2) | (1 << 1)));
}

bool IsHDA(u32 bus, u32 dev, u32 function)
{
	u32 id, type;

	id = ReadPCI(bus, dev, function, 0);
	if (id == 0xffff)
		return false;

	type = (ReadPCI(bus, dev, function, 0x08) >> 8);
	return (type == 0x040300);
}

u32 ReadPCI(u32 bus, u32 dev, u32 function, u32 offset)
{
	OutL(CONFIG_ADDR, 
			(0x80000000 
			 | (bus << 16) 
			 | (dev << 11) 
			 | (function << 8) 
			 | (offset)));
	return InL(CONFIG_DATA);
}

void HDAFound(u32 bus, u32 dev, u32 function)
{
	Print("HDA Found!\n", GREEN);
	if(hda_sc_ptr >= 10)
		return;
	hda_sc[hda_sc_ptr].present = true;
	hda_sc[hda_sc_ptr].base = PCIReadMMIOBar(bus, dev, function, PCI_BAR0);
	hda_sc[hda_sc_ptr].communication = HDA_UNINITIALIZED;
	PCIEnableMMIOBusMastering(bus, dev, function);
}
