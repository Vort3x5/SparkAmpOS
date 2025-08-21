#pragma once

#include <stdtypes.h>

#define SAMPLE_RATE  44100
#define FREQUENCY    440
#define AMPLITUDE    30000

#define BUFFER_SIZE  4096

#ifndef DEMO_DEF

static s16 *demo_audio;

#endif

void Brk();

void AudioInit();
void GenerateSineWave();
