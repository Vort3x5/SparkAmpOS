#include <stdtypes.h>
#include <utils.h>
#include <math.h>
#include <video.h>
#include <interrupts.h>
#include <clock.h>
#include <keyboard.h>
#include <memory.h>
#include <pci.h>
#include <ac97.h>

void Main() 
{
	InitDMem();

	TTYReset();
	Clear();
	Print(BLUE, "Start!\n", 5, 3, 6);

	IDTInstall();
	ISRsInstall();
	IRQsInstall();
	__asm__("sti");
	Print(GREEN, "Interrupts Installed\n");

	ScanPCI();
	PrintSepration();

	AC97Init();

	GenerateSineWave();
	AC97Play();

	Print(GREEN, "AC97 Sound Card Initialized\n");
	PrintSepration();

	AC97StartAmp();
	AmpLoop();

	Print(BLUE, "Finish!");
	_Halt();
}
