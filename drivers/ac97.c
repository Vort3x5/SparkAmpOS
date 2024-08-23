#include <ac97.h>

#include <video.h>
#include <pci.h>

bool PCIIsAC97(u32 bus, u32 dev, u32 function)
{
	u32 id, type;

	id = ReadPCI(bus, dev, function, 0);
	if (id == 0xffff)
		return false;

	type = (ReadPCI(bus, dev, function, 0x08) >> 8);
	return (type == 0x040100);
}

void PCIAC97Found(u32 bus, u32 dev, u32 function)
{
	Print("AC97 Found!\n", GREEN);
	ac97_base = PCIReadMMIOBar(bus, dev, function, PCI_BAR0);
	PCIEnableMMIOBusMastering(bus, dev, function);
}
