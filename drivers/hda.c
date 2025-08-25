#define HDA_DEF
#include <hda.h>

#include <io.h>
#include <pci.h>
#include <memory.h>
#include <video.h>
#include <interrupts.h>

bool PCIIsHDA(u32 bus, u32 dev, u32 function)
{
	u32 id, type;

	id = ReadPCI(bus, dev, function, 0);
	if (id == 0xffff)
		return false;

	type = (ReadPCI(bus, dev, function, 0x08) >> 8);
	return (type == 0x040300);
}

void PCIHDAFound(u32 bus, u32 dev, u32 function)
{
	Print("HDA Found!\n", GREEN);
	if(hda_sc_ptr >= 10)
		return;
	hda_sc[hda_sc_ptr].present = true;
	hda_sc[hda_sc_ptr].base = PCIReadMMIOBar(bus, dev, function, PCI_BAR0);
	hda_sc[hda_sc_ptr].communication = HDA_UNINITIALIZED;
	PCIEnableMMIOBusMastering(bus, dev, function);
}

void HDAInit()
{
	HDAReset();

	CORBInit();
	PrintSepration();
	RIRBInit();

	PrintSepration();
	MMOut16(hda_base + HDA_REG_INTCNTL, 0);
	Print("HDA Interrupts Disabled\n", GREEN);

	HDAIdentifyCodecs();
}

void HDAReset()
{
	hda_base = hda_sc[hda_sc_ptr].base;

	MMOut16(hda_base + HDA_REG_CTLR, 0);
	while (MMIn16(hda_base + HDA_REG_CTLR) & 1);

	MMOut16(hda_base + HDA_REG_CTLR, 1);
	while (!(MMIn16(hda_base + HDA_REG_CTLR) & 1));
}

void CORBInit()
{
	MMOut8(hda_base + HDA_REG_CORBCTL, 0x0);

	// corb_base = AlignedMalloc(256 * sizeof(u32), 128);
	MMOut32(hda_base + HDA_REG_CORBLBASE, (u32)corb_base);
	MMOut32(hda_base + HDA_REG_CORBUBASE, 0);

	u8 corb_entries_info;
	if ((MMIn8(hda_base + HDA_REG_CORBSIZE) & 0x40) == 0x40)
		corb_entries = 256, corb_entries_info = 0x2;
	else if ((MMIn8(hda_base + HDA_REG_CORBSIZE) & 0x20) == 0x20)
		corb_entries = 16, corb_entries_info = 0x1;
	else if ((MMIn8(hda_base + HDA_REG_CORBSIZE) & 0x10) == 0x10)
		corb_entries = 2, corb_entries_info = 0;
	else
	{
		Print("ERROR: CORB Not Supported\n", RED);
		_Halt();
	}
	Print("CORB Entries Number: ", WHITE);
	PrintNum(corb_entries, LIGHT_CYAN);
	PutC('\n', WHITE);
	MMOut8(hda_base + HDA_REG_CORBSIZE, corb_entries_info);

	MMOut16(hda_base + HDA_REG_CORBRP, 0x8000);
	while ((MMIn16(hda_base + HDA_REG_CORBRP) & 0x8000) != 0x8000);
	MMOut16(hda_base + HDA_REG_CORBRP, 0);
	while ((MMIn16(hda_base + HDA_REG_CORBRP) & 0x8000) != 0);
	Print("CORB Read Pointer Reset Success!\n", GREEN);

	MMOut16(hda_base + HDA_REG_CORBWP, 0);
	corb_ptr = 1;
	Print("CORB Write Pointer Reset Success!\n", GREEN);

	MMOut8(hda_base + HDA_REG_CORBCTL, 0x2);
	Print("CORB base address: ", WHITE);
	PrintNum(corb_base, LIGHT_CYAN);
	PutC('\n', WHITE);
}

void RIRBInit()
{
	MMOut8(hda_base + HDA_REG_RIRBCTL, 0x0);

	// rirb_base = AlignedMalloc(256 * sizeof(u64), 128);
	MMOut32(hda_base + HDA_REG_RIRBLBASE, (u64)rirb_base);
	MMOut32(hda_base + HDA_REG_RIRBUBASE, 0);

	u8 rirb_entries_info;
	if ((MMIn8(hda_base + HDA_REG_RIRBSIZE) & 0x40) == 0x40)
		rirb_entries = 256, rirb_entries_info = 0x2;
	else if ((MMIn8(hda_base + HDA_REG_RIRBSIZE) & 0x20) == 0x20)
		rirb_entries = 16, rirb_entries_info = 0x1;
	else if ((MMIn8(hda_base + HDA_REG_RIRBSIZE) & 0x10) == 0x10)
		rirb_entries = 2, rirb_entries_info = 0;
	else
	{
		Print("ERROR: RIRB Not Supported\n", RED);
		_Halt();
	}
	Print("RIRB Entries Number: ", WHITE);
	PrintNum(rirb_entries, LIGHT_CYAN);
	PutC('\n', WHITE);
	MMOut8(hda_base + HDA_REG_RIRBSIZE, rirb_entries_info);

	MMOut16(hda_base + HDA_REG_RIRBWP, 0x8000);
	rirb_ptr = 1;
	Print("RIRB Write Pointer Reset Success!\n", GREEN);

	MMOut8(hda_base + HDA_REG_RIRBCTL, 0x2);
	Print("RIRB base address: ", WHITE);
	PrintNum(rirb_base, LIGHT_CYAN);
	PutC('\n', WHITE);
}

void HDAIdentifyCodecs()
{
	MMOut32(hda_base + HDA_REG_WAKEEN, HDA_REG_WAKEEN_ENABLE);
	u16 statests = MMIn16(hda_base + HDA_REG_STATESTS);

	Print("statests register value (number of avaliable codecs): ", WHITE);
	PrintNum(statests, LIGHT_CYAN);
	PutC('\n', WHITE);

	for (s32 i = 0; i < 15; ++i)
	{
		if (statests & (1 << i))
		{
			PrintNum(i, LIGHT_CYAN);
			PutC('\n', WHITE);
		}
	}
}

u64 HDACmdResponse(u32 codec, u32 node, u32 verb, u32 cmd)
{
	u32 msg = ((codec << 28) | (node << 20) | (verb << 8) | (cmd));
	u32 *corb_mem = (u32 *)corb_base;

	PrintNum(corb_base, LIGHT_CYAN);
	PutC('\n', WHITE);
	PrintNum((u32)&(corb_mem[corb_ptr]), LIGHT_CYAN);
	PutC('\n', WHITE);
	corb_mem[corb_ptr] = msg;
	MMOut16(hda_base + HDA_REG_CORBWP, corb_ptr);

	for (s32 i = 0; i < 10; ++i)
		if (MMIn16(hda_base + HDA_REG_RIRBWP) == corb_ptr)
			break;

	if (MMIn16(hda_base + HDA_REG_RIRBWP) != corb_ptr)
	{
		Print("ERROR: No Response\n", RED);
		_Halt();
	}
	return 0;
}

void HDAConfigCodec()
{
	HDACmdResponse(0, 0xf00, 0, 0);
	HDACmdResponse(0, 0xf01, 0, 0);
}

void HDAConfigOutStream(u32 buff_addr, u32 buff_size)
{
	MMOut32(hda_base + HDA_REG_SDBDPL(HDA_STREAM_ID), (u32)(buff_addr & 0xffffffff));
	MMOut32(hda_base + HDA_REG_SDBDPU(HDA_STREAM_ID), (u32)((buff_addr >> 32) & 0xffffffff));

	MMOut32(hda_base + HDA_REG_SDCBL(HDA_STREAM_ID), buff_size);

	MMOut16(hda_base + HDA_REG_SDLVI(HDA_STREAM_ID), (buff_size / 4096) - 1);

	MMOut16(hda_base + HDA_REG_SDFMT(HDA_STREAM_ID), HDA_STREAM_FORMAT);

	MMOut32(hda_base + HDA_REG_SDCTL(HDA_STREAM_ID), (HDA_STREAM_ID << HDA_REG_SDCTL_STREAM_NUMBER) | 0x02);
}

void LoadAudioData(u32 audio_buff)
{
	for (s32 i = 0; i < AUDIO_SAMPLE_SIZE; ++i)
		((u8 *)audio_buff)[i] = (i % 256) - 128;
}

void StartAudioPlayback()
{
	MMOut32(hda_base + HDA_REG_SDCTL(HDA_STREAM_ID), 
			MMIn32(hda_base + HDA_REG_SDCTL(HDA_STREAM_ID)) | 0x02);
}
