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
