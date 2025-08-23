#define DEMO_DEF
#include <demo.h>

#include <stdtypes.h>
#include <utils.h>
#include <math.h>
#include <memory.h>
#include <video.h>
#include <interrupts.h>

s16 *demo_audio;

void GenerateSineWave()
{
	demo_audio = ALLOC_ARRAY(&g_noreset_buffer, s16, BUFFER_SIZE);
	PrintNum((u64)demo_audio, LIGHT_CYAN);
	PutC('\n', WHITE);
	for (s32 i = 0; i < BUFFER_SIZE; ++i)
	{
		f64 t = (f64)i / SAMPLE_RATE;
		f64 sample = AMPLITUDE * Sin(2.0 * PI * FREQUENCY * t);
		demo_audio[i] = (s16)sample;
	}
}
