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

#define BUS_REG_PCM_IN   0x00
#define BUS_REG_PCM_OUT  0x10
#define BUS_REG_MIC      0x20
#define BUS_REG_GCTLR    0x2c
#define BUS_REG_GSTS     0x30

static u64 nam_base, nabm_base;
static u8 channel_capabilities, sample_capabilities;
static bool ext_capabilities;

bool PCIIsAC97(u32 bus, u32 dev, u32 function);
void PCIAC97Found(u32 bus, u32 dev, u32 function);

void AC97Init();
