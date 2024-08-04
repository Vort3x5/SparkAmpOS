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

static u32 corb_base, corb_entries;

static u32 rirb_base;

void HDAInit()
{
	HDAReset();

	MMOutL(hda_base + HDA_REG_INTCNTL, 0);

	CORBInit();

	rirb_base = Malloc(HDA_REG_RIRBSIZE * (sizeof(u32)));
	MMOutL(hda_base + HDA_REG_RIRBLBASE, (u32)(rirb_base & 0xffffffff));
	MMOutL(hda_base + HDA_REG_RIRBUBASE, (u32)((rirb_base >> 32) & 0xffffffff));
	MMOutB(hda_base + HDA_REG_RIRBSIZE, 0x02);
	MMOutB(hda_base + HDA_REG_RIRBCTL, 0x02);
	Print("RIRB base address: ", WHITE);
	PrintNum(rirb_base, LIGHT_CYAN);

	HDAIdentifyCodecs();

	MMOutW(hda_base + HDA_REG_RIRBWP, 0xffff);

	u8 rirbctl_curr_val = MMInB(hda_base + HDA_REG_RIRBCTL);
	MMOutB(hda_base + HDA_REG_RIRBCTL, rirbctl_curr_val | 1);
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
	Print("CORB Write Pointer Reset Success!\n", GREEN);

	// MMOutB(hda_base + HDA_REG_CORBCTL, 0x02);
	Print("CORB base address: ", WHITE);
	PrintNum(corb_base, LIGHT_CYAN);
}

void RIRBInit()
{
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

void HDASendCommand(u32 command)
{
	u16 corb_wp = MMInW(hda_base + HDA_REG_CORBWP);
	corb_wp = ((corb_wp + 1) * sizeof(u32)) % HDA_REG_CORBSIZE;
	MMOutW(hda_base + HDA_REG_CORBWP, corb_wp);

	((u32 *)corb_base)[corb_wp] = command;

	MMOutB(hda_base + HDA_REG_CORBCTL, MMInB(hda_base + HDA_REG_CORBCTL) | 0x01);
}

u32 HDAReadResponse()
{
	while (!(MMInB(hda_base + HDA_REG_RIRBSTS) & 0x01));
	Print("response ready", GREEN);
	_Halt();
	u16 rirb_wp = MMInW(hda_base + HDA_REG_RIRBWP);

	u32 response = ((u32 *)rirb_base)[rirb_wp];
	rirb_wp = (rirb_wp + 1) % HDA_REG_RIRBSIZE;
	MMOutW(hda_base + HDA_REG_RIRBWP, rirb_wp);
	return response;
}

void HDACodecCommand(u32 verb, u32 payload)
{
	u32 cmd = 
		(HDA_CODEC_ADDRESS << 28) 
		| (HDA_CODEC_NODE_ID << 20) 
		| (verb << 8) 
		| (payload & 0xff);
	HDASendCommand(cmd);
}

void HDAConfigCodec()
{
	HDACodecCommand(0xf00, 0x00);
	HDACodecCommand(0xf01, 0x00);
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
