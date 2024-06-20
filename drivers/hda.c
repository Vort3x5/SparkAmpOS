#define HDA_DEF
#include <hda.h>

#include <io.h>
#include <pci.h>
#include <memory.h>

HDA hda_sc[10];
u32 hda_sc_ptr;
u32 selected_hda;

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

	u32 corb_base = (u32) Malloc(HDA_REG_CORBSIZE * (sizeof(u32)));
	MMOutL(hda_base + HDA_REG_CORBLBASE, (u32)(corb_base & 0xffffffff));
	MMOutL(hda_base + HDA_REG_CORBUBASE, (u32)((corb_base >> 32) & 0xffffffff));
	MMOutB(hda_base + HDA_REG_CORBSIZE, 0x02);
	MMOutB(hda_base + HDA_REG_CORBCTL, 0x02);

	u32 rirb_base = (u32) Malloc(HDA_REG_RIRBSIZE * (sizeof(u32)));
	MMOutL(hda_base + HDA_REG_RIRBLBASE, (u32)(rirb_base & 0xffffffff));
	MMOutL(hda_base + HDA_REG_RIRBUBASE, (u32)((rirb_base >> 32) & 0xffffffff));
	MMOutB(hda_base + HDA_REG_RIRBSIZE, 0x02);
	MMOutB(hda_base + HDA_REG_RIRBCTL, 0x02);

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

	u32 corb_base = 
		MMInL(hda_base + HDA_REG_CORBLBASE) 
		| ((u32) MMInL(hda_base + HDA_REG_CORBUBASE) << 32);

	((u32 *)corb_base)[corb_wp] = command;

	MMOutB(hda_base + HDA_REG_CORBCTL, MMInB(hda_base + HDA_REG_CORBCTL) | 0x01);
}

u32 HDAReadResponse()
{
	u32 hda_base = hda_sc[hda_sc_ptr].base;

	while (!(MMInB(hda_base + HDA_REG_RIRBSTS) & 0x01));
	u16 rirb_wp = MMInW(hda_base + HDA_REG_RIRBWP);

	u32 rirb_base = 
		MMInL(hda_base + HDA_REG_RIRBLBASE) 
		| ((u32) MMInL(hda_base + HDA_REG_RIRBUBASE) << 32);

	u32 response = ((u32 *)rirb_base)[rirb_wp];
	rirb_wp = (rirb_wp + 1) % HDA_REG_RIRBSIZE;
	MMOutW(hda_base + HDA_REG_RIRBWP, rirb_wp);
	return response;
}
