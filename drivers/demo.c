#define DEMO_DEF
#include <demo.h>

#include <stdtypes.h>
#include <math.h>
#include <memory.h>
#include <video.h>
#include <interrupts.h>

s16 *audio_buffer;

void GenerateSineWave()
{
	audio_buffer = (s16 *)Malloc(sizeof(s16) * 80);
	PrintNum((u64)audio_buffer, LIGHT_CYAN);
	for (s32 i = 0; i < 80; ++i)
	{
		f64 t = (f64)i / SAMPLE_RATE;
		f64 sample = AMPLITUDE * Sin(2.0 * PI * FREQUENCY * t);
		audio_buffer[i] = (s16)sample;
	}
	Brk();
}

void Brk()
{
	Print("Breakpoint!\n", YELLOW);
	_Halt();
}
