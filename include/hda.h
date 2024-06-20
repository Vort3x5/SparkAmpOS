#pragma once

#include <stdtypes.h>

#define HDA_UNINITIALIZED   0x0
#define HDA_CORB_RIRB       0x1
#define HDA_PIO             0x2

#define HDA_REG_CTLR        0x08

#define HDA_REG_CORBLBASE   0x40
#define HDA_REG_CORBUBASE   0x44
#define HDA_REG_CORBWP      0x48
#define HDA_REG_CORBRP      0x4A
#define HDA_REG_CORBCTL     0x4C
#define HDA_REG_CORBSTS     0x4D
#define HDA_REG_CORBSIZE    0x4E

#define HDA_REG_RIRBLBASE   0x50
#define HDA_REG_RIRBUBASE   0x54
#define HDA_REG_RIRBWP      0x58

#define HDA_REG_INTCNTL     0x5A

#define HDA_REG_RIRBCTL     0x5C
#define HDA_REG_RIRBSTS     0x5D
#define HDA_REG_RIRBSIZE    0x5E

typedef struct {
	bool present;
	u32 base;
	u32 input_stream_base;
	u32 output_stream_base;
	u32 communication;
	u8 codec_number;

	u32 afg_node_sample_capabilites;
	u32 afg_node_stream_format_capabilites;
	u32 afg_node_input_amp_capabilites;
	u32 afg_node_output_amp_capabilites;

	u8 audio_output_node_number; 
	u32 audio_output_node_sample_capabilites;
	u32 audio_output_node_stream_format_capabilites;

	u8 output_amp_node_number;
	u32 output_amp_node_capabilites;
} HDA;

#ifndef HDA_DEF

extern HDA hda_sc[10];
extern u32 hda_sc_ptr;
extern u32 selected_hda;

#endif

void HDAReset();
void HDAInit();
