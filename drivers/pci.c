#include <pci.h>

#include <stdtypes.h>
#include <io.h>
#include <video.h>
#include <interrupts.h>
#include <hda.h>
#include <ac97.h>

void ScanPCI()
{
	// hda_sc_ptr = 0;
	for (s32 bus = 0; bus < 256; ++bus)
	{
		for (s32 dev = 0; dev < 32; ++dev)
		{
			if (PCIIsAC97(bus, dev, 0))
			{
				PCIAC97Found(bus, dev, 0);
				return;
			}

			if ((ReadPCI(bus, dev, 0, 0x0C) & 0x00800000) == 0x00800000)
			{
				for (s32 function = 1; function < 8; ++function)
				{
					if (PCIIsAC97(bus, dev, function))
					{
						PCIAC97Found(bus, dev, 0);
						return;
					}
				}
			}
		}
	}
	Print("Fatal: AC97 not found", RED);
	_Halt();
}

void WritePCI(u32 bus, u32 dev, u32 function, u32 offset, u32 val)
{
	Out32(CONFIG_ADDR, (0x80000000 | (bus << 16) | (dev << 11) | (function << 8) | (offset)));
	Out32(CONFIG_DATA, val);
}

u32 PCIReadMMIOBar(u32 bus, u32 dev, u32 function, u32 bar)
{
	Out32(CONFIG_ADDR, 
			(0x80000000 
			 | (bus << 16) 
			 | (dev << 11) 
			 | (function << 8) 
			 | (bar)));
	return (In32(CONFIG_DATA) & 0xfffffff0);
}

void PCIEnableMMIOBusMastering(u32 bus, u32 dev, u32 function)
{
	WritePCI(bus, dev, function, 0x04, 
			((ReadPCI(bus, dev, function, 0x04) 
			  & ~(1 << 10)) | (1 << 2) | (1 << 1)));
}

u32 ReadPCI(u32 bus, u32 dev, u32 function, u32 offset)
{
	Out32(CONFIG_ADDR, 
			(0x80000000 
			 | (bus << 16) 
			 | (dev << 11) 
			 | (function << 8) 
			 | (offset)));
	return In32(CONFIG_DATA);
}
