#pragma once

#include <stdtypes.h>

#define PI 3.14159265358979323846

#define SAMPLE_RATE  44100
#define FREQUENCY    440
#define AMPLITUDE    30000

#define BUFFER_SIZE  4096
 
f64 Sin(f64 x);
void GenerateSineWave();
