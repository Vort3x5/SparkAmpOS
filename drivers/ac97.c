#define AC97_DEF
#include <ac97.h>

#include <video.h>
#include <utils.h>
#include <math.h>
#include <pci.h>
#include <io.h>
#include <memory.h>
#include <interrupts.h>
#include <video.h>

extern s16 *demo_audio;

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
	PrintNum(sample_capabilities, LIGHT_CYAN);
	PutC('\n', WHITE);

	ext_capabilities = (MMInW(nam_base + MIXER_EXT_CAPABILITIES) & 0x08)
		&& (MMInW(nam_base + MIXER_AUX_OUT_VOL) == 0x8000);
	Print("Headphone output boolean value: ", WHITE);
	PrintNum(ext_capabilities, LIGHT_CYAN);
	PutC('\n', WHITE);

	bdl_ptr = 
		(struct BDL_Entry *)ArenaAlloc(&g_noreset_buffer, (sizeof (struct BDL_Entry)) * 32, 16);
	Print("BDL address: ", WHITE);
	PrintNum((u64)bdl_ptr, LIGHT_CYAN);
	PutC('\n', WHITE);

	// Mute at init
	MMOutW(nam_base + MIXER_PCM_OUT_VOL, 0);
	MMOutW(nam_base + MIXER_MASTER_OUT_VOL, 0);
}

void AC97Play()
{
	MMOutW(nam_base + MIXER_PCM_OUT_VOL, 0x808);
	MMOutW(nam_base + MIXER_MASTER_OUT_VOL, 0x808);

	FillBDL();

	MMOutB(nabm_base + BUS_REG_RESET, 0x02);
	while((MMInB(nabm_base + BUS_REG_RESET) & 0x2) == 0x2)
		asm("nop");
BRK();

	MMOutL(nabm_base + BUS_PCM_OUT_BOX + BUS_ADDR_OF_BDL, (u32)bdl_ptr);
	MMOutB(nabm_base + BUS_PCM_OUT_BOX + BUS_NUM_OF_BD_ENTRIES, 1);

	MMOutB(nabm_base + BUS_REG_RESET, 0x01);

	// Start the PCM output stream
	MMOutW(nam_base + BUS_PCM_OUT_BOX + BUS_TRANSFER_CTRL, 0x15);
}

void FillBDL()
{
	bdl_ptr[curr_entry] = (struct BDL_Entry) { 
		.addr = (u32)demo_audio,
		.num_of_samples = BUFFER_SIZE,
		.reserved = 0,
		.last_buffer_entry = 1,
		.int_on_completion = 1
	};
	curr_entry = (curr_entry + 1) & (NUM_OF_BDL_ENTRIES - 1);
}
