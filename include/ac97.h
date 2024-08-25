#pragma once

#include <stdtypes.h>

#define MIXER_REG_RESET              0x00
#define MIXER_MASTER_OUT_VOL         0x02
#define MIXER_AUX_OUT_VOL            0x04
#define MIXER_MIC_VOL                0x0e
#define MIXER_PCM_OUT_VOL            0x18
#define MIXER_IN_DEV                 0x1a
#define MIXER_IN_GAIN                0x1c
#define MIXER_GAIN_OF_MIC            0x1e
#define MIXER_EXT_CAPABILITIES       0x28
#define MIXER_CTRL_EXT_CAPABILITIES  0x2a
#define MIXER_PCM_FRONT_DAC          0x2c
#define MIXER_PCM_SUR_DAC            0x2e
#define MIXER_PCM_LFE_DAC            0x30
#define MIXER_PCM_LR_ADC             0x32

#define BUS_ADDR_OF_BDL                      0x00
#define BUS_NUM_OF_PROCESSED_BD_ENTRY        0x04
#define BUS_NUM_OF_BD_ENTRIES                0x05
#define BUS_STS_OF_DATA_TRANSFER             0x06
#define BUS_NUM_OF_TRANSFERRED_SAMPLES       0x08
#define BUS_NUM_OF_NEXT_PROCESSED_BDL_ENTRY  0x0a
#define BUS_TRANSFER_CTRL                    0x0b
#define BUS_REG_MIC                          0x20
#define BUS_REG_GCTLR                        0x2c
#define BUS_REG_GSTS                         0x30

struct BDL_Entry
{
	u32 addr;
	u16 num_of_samples, flags;
}__attribute__((packed));

#ifdef AC97_DEF

static u64 nam_base, nabm_base;
static u8 channel_capabilities, sample_capabilities;
static bool ext_capabilities;

#endif

bool PCIIsAC97(u32 bus, u32 dev, u32 function);
void PCIAC97Found(u32 bus, u32 dev, u32 function);

void AC97Init();
void FillBDL();
