#pragma once

#include <stdtypes.h>

#define SAMPLE_RATE  44100
#define FREQUENCY    440
#define AMPLITUDE    30000
#define BUFFER_SIZE  65534
// #define BUFFER_SIZE  10000 Works

#ifndef DEMO_DEF

extern s16 *audio_buffer;

#endif

void Brk();

void GenerateSineWave();
