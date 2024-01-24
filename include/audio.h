#pragma once

#include <stdtypes.h>

#define HDA_UNINITIALIZED 0
#define HDA_CORB_RIRB 1
#define HDA_PIO 2

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
