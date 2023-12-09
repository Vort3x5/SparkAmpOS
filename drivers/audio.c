#include <audio.h>

#include <stdtypes.h>
#include <io.h>
#include <clock.h>

void PlaySound(u32 frequence)
{
	u32 div;
	byte tmp;

	div = 1193180 / frequence;
	OutPortB(0x43, 0xb6);
	OutPortB(0x42, (byte) (div));
	OutPortB(0x42, (byte) (div >> 8));

	tmp = InPortB(0x61);
	if (tmp != (tmp | 3))
		OutPortB(0x61, tmp | 3);
}

void NoSound()
{
	byte tmp = InPortB(0x61) & 0xFC;

	OutPortB(0x61, tmp);
}

void Beep()
{
	PlaySound(1000);
	Sleep(2000);
	NoSound();
}
