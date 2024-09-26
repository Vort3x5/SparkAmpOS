#define AC97_DEF
#include <ac97.h>

#include <video.h>
#include <pci.h>
#include <io.h>
#include <memory.h>
#include <interrupts.h>
#include <video.h>

#include <demo.h>

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
	PutC('\n', WHITE);

	sample_capabilities = 
		(MMInL(nabm_base + BUS_REG_GSTS) >> 22) & 0x03;
	Print("Sample capabilities bits: ", WHITE);
	PrintNum(channel_capabilities, LIGHT_CYAN);
	PutC('\n', WHITE);

	ext_capabilities = (MMInW(nam_base + MIXER_EXT_CAPABILITIES) & 0x08)
		&& (MMInW(nam_base + MIXER_AUX_OUT_VOL) == 0x8000);
	Print("Headphone output boolean value: ", WHITE);
	PrintNum(ext_capabilities, LIGHT_CYAN);
	PutC('\n', WHITE);

	bdl_ptr = 
		(struct BDL_Entry *)AlignedMalloc(sizeof (struct BDL_Entry) * 32, 0x10);
	Print("BDL address: ", WHITE);
	PrintNum((u64)bdl_ptr, LIGHT_CYAN);
	PutC('\n', WHITE);

	MMOutW(nam_base + MIXER_PCM_OUT_VOL, 0);
}

void AC97Play()
{
	MMOutW(nam_base + MIXER_MASTER_OUT_VOL, 0);

	FillBDL();

	Brk();

	MMOutB(nabm_base + BUS_REG_RESET, 0x02);
	while((MMInB(nabm_base + BUS_REG_RESET) & 0x2) == 0x2)
		asm("nop");

	MMOutL(nabm_base + BUS_PCM_OUT_BOX + BUS_ADDR_OF_BDL, (u32)bdl_ptr);
	MMOutB(nabm_base + BUS_PCM_OUT_BOX + BUS_NUM_OF_BD_ENTRIES, 1);

	MMOutB(nabm_base + BUS_REG_RESET, 0x01);
}

void SetSampleRate(u16 sample_rate)
{
}

void FillBDL()
{
	bdl_ptr[curr_entry].addr = (u32)audio_buffer;
	bdl_ptr[curr_entry].num_of_samples = BUFFER_SIZE;
	bdl_ptr[curr_entry].reserved = 0;
	bdl_ptr[curr_entry].last_buffer_entry = 0;
	bdl_ptr[curr_entry].int_on_completion = 0;

	curr_entry = (curr_entry + 1) & (NUM_OF_BDL_ENTRIES - 1);
}
