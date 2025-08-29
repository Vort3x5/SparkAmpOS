#define AC97_DEF
#include <ac97.h>

#include <stdtypes.h>
#include <video.h>
#include <utils.h>
#include <math.h>
#include <pci.h>
#include <io.h>
#include <memory.h>
#include <interrupts.h>
#include <video.h>
#include <amp.h>

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
	Out32(nabm_base + BUS_REG_GCTLR, 0x2);
	Out16(nam_base + MIXER_REG_RESET, 0x1);

	channel_capabilities = 
		(In32(nabm_base + BUS_REG_GSTS) >> 20) & 0x03;
	Print("Channel capabilities bits: ", WHITE);
	PrintNum(channel_capabilities, LIGHT_CYAN);
	PutC('\n', WHITE);

	sample_capabilities = 
		(In32(nabm_base + BUS_REG_GSTS) >> 22) & 0x03;
	Print("Sample capabilities bits: ", WHITE);
	PrintNum(sample_capabilities, LIGHT_CYAN);
	PutC('\n', WHITE);

	ext_capabilities = (In16(nam_base + MIXER_EXT_CAPABILITIES) & 0x08)
		&& (In16(nam_base + MIXER_AUX_OUT_VOL) == 0x8000);
	Print("Headphone output boolean value: ", WHITE);
	PrintNum(ext_capabilities, LIGHT_CYAN);
	PutC('\n', WHITE);

	bdl_in = 
		(struct BDL_Entry *)ArenaAlloc(&g_noreset_buffer, (sizeof (struct BDL_Entry)) * 32, 16);

	bdl_out = 
		(struct BDL_Entry *)ArenaAlloc(&g_noreset_buffer, (sizeof (struct BDL_Entry)) * 32, 16);

	Out16(nam_base + MIXER_IN_GAIN, 0x0000);
	Out16(nam_base + MIXER_IN_DEV, 0x0404);

	// Mute at init
	Out16(nam_base + MIXER_PCM_OUT_VOL, 0);
	Out16(nam_base + MIXER_MASTER_OUT_VOL, 0);
}

// Only for Demo sine wave
void AC97Play()
{
	Out16(nam_base + MIXER_PCM_OUT_VOL, 0x808);
	Out16(nam_base + MIXER_MASTER_OUT_VOL, 0x808);

	FillOutBDL();

	Out8(nabm_base + BUS_REG_RESET, 0x02);
	while((In8(nabm_base + BUS_REG_RESET) & 0x2) == 0x2)
		asm("nop");

	Out32(nabm_base + BUS_PCM_OUT_BOX + BUS_ADDR_OF_BDL, (u32)bdl_out);
	Out8(nabm_base + BUS_PCM_OUT_BOX + BUS_NUM_OF_BD_ENTRIES, 1);

	Out8(nabm_base + BUS_REG_RESET, 0x01);

	// Start the PCM output stream
	Out16(nam_base + BUS_PCM_OUT_BOX + BUS_TRANSFER_CTRL, 0x15);
}

void FillInBDL()
{
	bdl_in[0] = (struct BDL_Entry) { 
		.addr = (u32)audio_in,
		.num_of_samples = BUFFER_SIZE,
		.reserved = 0,
		.last_buffer_entry = 1,
		.int_on_completion = 1
	};
}

void FillOutBDL()
{
	bdl_out[curr_entry] = (struct BDL_Entry) { 
		.addr = (u32)audio_out,
		.num_of_samples = BUFFER_SIZE,
		.reserved = 0,
		.last_buffer_entry = 1,
		.int_on_completion = 1
	};
	curr_entry = (curr_entry + 1) & (NUM_OF_BDL_ENTRIES - 1);
}

void AC97StartAmp()
{
	Out16(nam_base + MIXER_PCM_OUT_VOL, 0x808);
	Out16(nam_base + MIXER_MASTER_OUT_VOL, 0x808);

	FillInBDL();
	FillOutBDL();

	Out8(nabm_base + BUS_REG_RESET, 0x02);
	while ((In8(nabm_base + BUS_REG_RESET) & 0x2) == 0x2)
		asm("nop");

	Out32(nabm_base + BUS_PCM_OUT_BOX + BUS_ADDR_OF_BDL, (u32)bdl_out);
	Out8(nabm_base + BUS_PCM_OUT_BOX + BUS_NUM_OF_BD_ENTRIES, 1);

	Out32(nabm_base + BUS_PCM_IN_BOX + BUS_ADDR_OF_BDL, (u32)bdl_in);
	Out8(nabm_base + BUS_PCM_IN_BOX + BUS_NUM_OF_BD_ENTRIES, 1);

	Out8(nabm_base + BUS_REG_RESET, 0x01);

	Out16(nam_base + BUS_PCM_OUT_BOX + BUS_TRANSFER_CTRL, 0x15);
	Out16(nam_base + BUS_PCM_IN_BOX + BUS_TRANSFER_CTRL, 0x15);

	Print("Amp ready to loop!\n", GREEN);
}

void AmpLoop()
{
	while (true)
	{
		u8 input_status = In8(nabm_base + BUS_PCM_IN_BOX + BUS_STS_OF_DATA_TRANSFER);
		if (input_status & 0x04)
		{
			for (u32 i = 0; i < BUFFER_SIZE; ++i)
			{
				s32 amplified = audio_in[i] * 16;

				if (amplified > 32767)
					amplified = 32767;
				if (amplified < -32768)
					amplified = -32768;

				audio_out[i] = (s16)amplified;
			}

			Out8(nabm_base + BUS_PCM_IN_BOX + BUS_STS_OF_DATA_TRANSFER, 0x04);
			Out8(nabm_base + BUS_PCM_OUT_BOX + BUS_STS_OF_DATA_TRANSFER, 0x04);

			FillInBDL();
			FillOutBDL();
		}
	}
}
