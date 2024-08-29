#pragma once

#include <stdtypes.h>

#define SAMPLE_RATE  44100
#define FREQUENCY    440
#define AMPLITUDE    30000
#define BUFFER_SIZE  44100

#ifndef DEMO_DEF

extern s16 *audio_buffer;

#endif

void Brk();

void GenerateSineWave();
