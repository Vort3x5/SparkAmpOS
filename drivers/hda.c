#define HDA_DEF
#include <hda.h>

#include <io.h>
#include <pci.h>
#include <memory.h>
#include <video.h>

HDA hda_sc[10];
u32 hda_sc_ptr;
u32 selected_hda;

static u32 corb_base;
static u32 rirb_base;

void HDAReset()
{
	u32 base = hda_sc[hda_sc_ptr].base;

	MMOutW(base + HDA_REG_CTLR, 0);
	while (MMInW(base + HDA_REG_CTLR) & 1);

	MMOutW(base + HDA_REG_CTLR, 1);
	while (!(MMInW(base + HDA_REG_CTLR) & 1));
}

void HDAInit()
{
	u32 hda_base = hda_sc[hda_sc_ptr].base;
	HDAReset();

	MMOutL(hda_base + HDA_REG_INTCNTL, 0);

	corb_base = Malloc(HDA_REG_CORBSIZE * (sizeof(u32)));
	MMOutL(hda_base + HDA_REG_CORBLBASE, (u32)(corb_base & 0xffffffff));
	MMOutL(hda_base + HDA_REG_CORBUBASE, (u32)((corb_base >> 32) & 0xffffffff));
	MMOutB(hda_base + HDA_REG_CORBSIZE, 0x02);
	MMOutB(hda_base + HDA_REG_CORBCTL, 0x02);
	Print("CORB base address: ", WHITE);
	PrintNum(corb_base, GREEN);

	rirb_base = Malloc(HDA_REG_RIRBSIZE * (sizeof(u32)));
	MMOutL(hda_base + HDA_REG_RIRBLBASE, (u32)(rirb_base & 0xffffffff));
	MMOutL(hda_base + HDA_REG_RIRBUBASE, (u32)((rirb_base >> 32) & 0xffffffff));
	MMOutB(hda_base + HDA_REG_RIRBSIZE, 0x02);
	MMOutB(hda_base + HDA_REG_RIRBCTL, 0x02);
	Print("RIRB base address: ", WHITE);
	PrintNum(rirb_base, GREEN);

	MMOutW(hda_base + HDA_REG_RIRBWP, 0xffff);

	u8 rirbctl_curr_val = MMInB(hda_base + HDA_REG_RIRBCTL);
	MMOutB(hda_base + HDA_REG_RIRBCTL, rirbctl_curr_val | 1);
}

void HDASendCommand(u32 command)
{
	u32 hda_base = hda_sc[hda_sc_ptr].base;

	u16 corb_wp = MMInW(hda_base + HDA_REG_CORBWP);
	corb_wp = (corb_wp + 1) % HDA_REG_CORBSIZE;
	MMOutW(hda_base + HDA_REG_CORBWP, corb_wp);

	((u32 *)corb_base)[corb_wp] = command;

	MMOutB(hda_base + HDA_REG_CORBCTL, MMInB(hda_base + HDA_REG_CORBCTL) | 0x01);
}

u32 HDAReadResponse()
{
	u32 hda_base = hda_sc[hda_sc_ptr].base;

	while (!(MMInB(hda_base + HDA_REG_RIRBSTS) & 0x01));
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

void HDAIdentifyCodecs()
{
	u32 hda_base = hda_sc[hda_sc_ptr].base;
	MMOutL(hda_base + HDA_REG_WAKEEN, HDA_REG_WAKEEN_ENABLE);
	u16 statests = MMInW(hda_base + HDA_REG_STATESTS);

	Print("statests register value (number of avaliable codecs): ", WHITE);
	PrintNum(statests, BLUE);

	for (s32 i = 0; i < 15; ++i)
		if (statests & (1 << i))
			PrintNum(i, GREEN);
}

void HDAConfigCodec()
{
	HDACodecCommand(0xf00, 0x00);
	HDACodecCommand(0xf01, 0x00);
}

void HDAConfigOutStream(u32 buff_addr, u32 buff_size)
{
	u32 hda_base = hda_sc[hda_sc_ptr].base;

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
	u32 hda_base = hda_sc[hda_sc_ptr].base;
	MMOutL(hda_base + HDA_REG_SDCTL(HDA_STREAM_ID), 
			MMInL(hda_base + HDA_REG_SDCTL(HDA_STREAM_ID)) | 0x02);
}
