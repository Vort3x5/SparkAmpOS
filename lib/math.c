#include <math.h>

#include <utils.h>
#include <video.h>
#include <memory.h>

s16 *demo_audio;

f64 Sin(f64 x)
{
	while (x > PI)
		x -= 2 * PI;
	while (x < -PI)
		x += 2 * PI;

	f64 res = 0.0, term = x;
	s32 n = 1;

	while (term > 0.00001 || term < -0.00001)
	{
		res += term;
		n += 2;
		term = -term * x * x / (n * (n - 1));
	}

	return res;
}

void GenerateSineWave()
{
	demo_audio = ALLOC_ARRAY(&g_noreset_buffer, s16, BUFFER_SIZE);
	PrintNum((u64)demo_audio, LIGHT_CYAN);
	PutC('\n', WHITE);
	for (u32 i = 0; i < BUFFER_SIZE; ++i)
	{
		f64 t = (f64)i / SAMPLE_RATE;
BRK();
		f64 sample = AMPLITUDE * Sin(2.0 * PI * FREQUENCY * t);
		demo_audio[i] = (s16)sample;
	}
}
