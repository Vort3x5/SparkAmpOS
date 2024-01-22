#include <audio.h>

#include <stdtypes.h>
#include <io.h>
#include <clock.h>

void PlaySound(u32 frequence)
{
	u32 div;
	byte tmp;

	div = 1193180 / frequence;
	OutB(0x43, 0xb6);
	OutB(0x42, (byte) (div));
	OutB(0x42, (byte) (div >> 8));

	tmp = InB(0x61);
	if (tmp != (tmp | 3))
		OutB(0x61, tmp | 3);
}

void NoSound()
{
	byte tmp = InB(0x61) & 0xFC;

	OutB(0x61, tmp);
}

void Beep()
{
	PlaySound(1000);
	Sleep(2000);
	NoSound();
}
