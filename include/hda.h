#pragma once

#include <stdtypes.h>

#define HDA_UNINITIALIZED   0x0
#define HDA_CORB_RIRB       0x01
#define HDA_PIO             0x02

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


#define HDA_REG_STATESTS            0x0E
#define HDA_REG_WAKEEN              0x0C
#define HDA_REG_WAKEEN_ENABLE       0xFFFF
#define HDA_REG_SDCTL_STREAM_NUMBER 20

#define HDA_REG_SDLPIB(stream)   (0x8 + stream * 0x20)
#define HDA_REG_SDCBL(stream)    (0x10 + stream * 0x20)
#define HDA_REG_SDLVI(stream)    (0x14 + stream * 0x20)
#define HDA_REG_SDCTL(stream)    (0x20 + stream * 0x20)
#define HDA_REG_SDFMT(stream)    (0x12 + stream * 0x20)
#define HDA_REG_SDBDPL(stream)   (0x18 + stream * 0x20)
#define HDA_REG_SDBDPU(stream)   (0x1C + stream * 0x20)

#define HDA_CORBCTL_RUN 0x02
#define HDA_RIRBCTL_RUN 0x02

#define HDA_CODEC_ADDRESS 0
#define HDA_CODEC_NODE_ID 0

#define HDA_STREAM_ID 1
#define HDA_STREAM_FORMAT 0x0011

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

void HDASendCommand(u32 command);
u32 HDAReadResponse();

void HDACodecCommand(u32 verb, u32 payload);
void HDAIdentifyCodecs();
void HDAConfigCodec();

void HDAConfigOutStream(u32 buff_addr, u32 buff_size);

#define AUDIO_SAMPLE_SIZE 8192
void LoadAudioData(u32 buff_addr);
void StartAudioPlayback();
