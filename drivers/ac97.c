#include <ac97.h>

#include <video.h>
#include <pci.h>
#include <io.h>

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
	nam_base = PCIReadMMIOBar(bus, dev, function, PCI_BAR0);
	nabm_base = PCIReadMMIOBar(bus, dev, function, PCI_BAR1);
	PCIEnableMMIOBusMastering(bus, dev, function);
}

void AC97Init()
{
	MMOutL(nabm_base + BUS_REG_GCTLR, 0x2);
	MMOutW(nam_base + MIXER_REG_RESET, 0x1);

	channel_capabilities = 
		(MMInL(nabm_base + BUS_REG_GSTS) >> 20) & 0x03;
	Print("Channel capabilities bits: ", WHITE);
	PrintNum(channel_capabilities, LIGHT_CYAN);

	sample_capabilities = 
		(MMInL(nabm_base + BUS_REG_GSTS) >> 22) & 0x03;
	Print("Sample capabilities bits: ", WHITE);
	PrintNum(channel_capabilities, LIGHT_CYAN);

	ext_capabilities = (MMInW(nam_base + MIXER_EXT_CAPABILITIES) & 0x08)
		&& (MMInW(nam_base + MIXER_AUX_OUT_VOL) == 0x8000);
	Print("Headphone output boolean value: ", WHITE);
	PrintNum(ext_capabilities, LIGHT_CYAN);

	MMOutW(nam_base + MIXER_PCM_OUT_VOL, 0);
}
