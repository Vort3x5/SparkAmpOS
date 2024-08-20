#define HDA_DEF
#include <hda.h>

#include <io.h>
#include <pci.h>
#include <memory.h>
#include <video.h>
#include <interrupts.h>

HDA hda_sc[10];
u32 hda_sc_ptr;
u32 selected_hda;

static u32 hda_base;

static u32 corb_base, corb_entries, rirb_entries, corb_ptr, rirb_ptr;
static u64 rirb_base;

void HDAInit()
{
	HDAReset();

	CORBInit();
	PrintSepration();
	RIRBInit();

	PrintSepration();
	MMOutW(hda_base + HDA_REG_INTCNTL, 0);
	Print("HDA Interrupts Disabled\n", GREEN);

	HDAIdentifyCodecs();
}

void HDAReset()
{
	hda_base = hda_sc[hda_sc_ptr].base;

	MMOutW(hda_base + HDA_REG_CTLR, 0);
	while (MMInW(hda_base + HDA_REG_CTLR) & 1);

	MMOutW(hda_base + HDA_REG_CTLR, 1);
	while (!(MMInW(hda_base + HDA_REG_CTLR) & 1));
}

void CORBInit()
{
	MMOutB(hda_base + HDA_REG_CORBCTL, 0x0);

	corb_base = AlignedMalloc(256 * sizeof(u32), 128);
	MMOutL(hda_base + HDA_REG_CORBLBASE, (u32)corb_base);
	MMOutL(hda_base + HDA_REG_CORBUBASE, 0);

	byte corb_entries_info;
	if ((MMInB(hda_base + HDA_REG_CORBSIZE) & 0x40) == 0x40)
		corb_entries = 256, corb_entries_info = 0x2;
	else if ((MMInB(hda_base + HDA_REG_CORBSIZE) & 0x20) == 0x20)
		corb_entries = 16, corb_entries_info = 0x1;
	else if ((MMInB(hda_base + HDA_REG_CORBSIZE) & 0x10) == 0x10)
		corb_entries = 2, corb_entries_info = 0;
	else
	{
		Print("ERROR: CORB Not Supported\n", RED);
		_Halt();
	}
	Print("CORB Entries Number: ", WHITE);
	PrintNum(corb_entries, LIGHT_CYAN);
	MMOutB(hda_base + HDA_REG_CORBSIZE, corb_entries_info);

	MMOutW(hda_base + HDA_REG_CORBRP, 0x8000);
	while ((MMInW(hda_base + HDA_REG_CORBRP) & 0x8000) != 0x8000);
	MMOutW(hda_base + HDA_REG_CORBRP, 0);
	while ((MMInW(hda_base + HDA_REG_CORBRP) & 0x8000) != 0);
	Print("CORB Read Pointer Reset Success!\n", GREEN);

	MMOutW(hda_base + HDA_REG_CORBWP, 0);
	corb_ptr = 1;
	Print("CORB Write Pointer Reset Success!\n", GREEN);

	MMOutB(hda_base + HDA_REG_CORBCTL, 0x2);
	Print("CORB base address: ", WHITE);
	PrintNum(corb_base, LIGHT_CYAN);
}

void RIRBInit()
{
	MMOutB(hda_base + HDA_REG_RIRBCTL, 0x0);

	rirb_base = AlignedMalloc(256 * sizeof(u64), 128);
	MMOutL(hda_base + HDA_REG_RIRBLBASE, (u64)rirb_base);
	MMOutL(hda_base + HDA_REG_RIRBUBASE, 0);

	byte rirb_entries_info;
	if ((MMInB(hda_base + HDA_REG_RIRBSIZE) & 0x40) == 0x40)
		rirb_entries = 256, rirb_entries_info = 0x2;
	else if ((MMInB(hda_base + HDA_REG_RIRBSIZE) & 0x20) == 0x20)
		rirb_entries = 16, rirb_entries_info = 0x1;
	else if ((MMInB(hda_base + HDA_REG_RIRBSIZE) & 0x10) == 0x10)
		rirb_entries = 2, rirb_entries_info = 0;
	else
	{
		Print("ERROR: RIRB Not Supported\n", RED);
		_Halt();
	}
	Print("RIRB Entries Number: ", WHITE);
	PrintNum(rirb_entries, LIGHT_CYAN);
	MMOutB(hda_base + HDA_REG_RIRBSIZE, rirb_entries_info);

	MMOutW(hda_base + HDA_REG_RIRBWP, 0x8000);
	rirb_ptr = 1;
	Print("RIRB Write Pointer Reset Success!\n", GREEN);

	MMOutB(hda_base + HDA_REG_RIRBCTL, 0x2);
	Print("RIRB base address: ", WHITE);
	PrintNum(rirb_base, LIGHT_CYAN);
}

void HDAIdentifyCodecs()
{
	MMOutL(hda_base + HDA_REG_WAKEEN, HDA_REG_WAKEEN_ENABLE);
	u16 statests = MMInW(hda_base + HDA_REG_STATESTS);

	Print("statests register value (number of avaliable codecs): ", WHITE);
	PrintNum(statests, LIGHT_CYAN);

	for (s32 i = 0; i < 15; ++i)
		if (statests & (1 << i))
			PrintNum(i, LIGHT_CYAN);
}

u64 HDACmdResponse(u32 codec, u32 node, u32 verb, u32 cmd)
{
	u32 msg = ((codec << 28) | (node << 20) | (verb << 8) | (cmd));
	u32 *corb_mem = (u32 *)corb_base;

	PrintNum(corb_base, LIGHT_CYAN);
	PrintNum((u32)&(corb_mem[corb_ptr]), LIGHT_CYAN);
	corb_mem[corb_ptr] = msg;
	MMOutW(hda_base + HDA_REG_CORBWP, corb_ptr);

	for (s32 i = 0; i < 10; ++i)
		if (MMInW(hda_base + HDA_REG_RIRBWP) == corb_ptr)
			break;

	if (MMInW(hda_base + HDA_REG_RIRBWP) != corb_ptr)
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
	MMOutL(hda_base + HDA_REG_SDBDPL(HDA_STREAM_ID), (u32)(buff_addr & 0xffffffff));
	MMOutL(hda_base + HDA_REG_SDBDPU(HDA_STREAM_ID), (u32)((buff_addr >> 32) & 0xffffffff));

	MMOutL(hda_base + HDA_REG_SDCBL(HDA_STREAM_ID), buff_size);

	MMOutW(hda_base + HDA_REG_SDLVI(HDA_STREAM_ID), (buff_size / 4096) - 1);

	MMOutW(hda_base + HDA_REG_SDFMT(HDA_STREAM_ID), HDA_STREAM_FORMAT);

	MMOutL(hda_base + HDA_REG_SDCTL(HDA_STREAM_ID), (HDA_STREAM_ID << HDA_REG_SDCTL_STREAM_NUMBER) | 0x02);
}

void LoadAudioData(u32 audio_buff)
{
	for (s32 i = 0; i < AUDIO_SAMPLE_SIZE; ++i)
		((u8 *)audio_buff)[i] = (i % 256) - 128;
}

void StartAudioPlayback()
{
	MMOutL(hda_base + HDA_REG_SDCTL(HDA_STREAM_ID), 
			MMInL(hda_base + HDA_REG_SDCTL(HDA_STREAM_ID)) | 0x02);
}
