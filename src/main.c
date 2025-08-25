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
	Print("Start!\n", BLUE);

	IDTInstall();
	ISRsInstall();
	IRQsInstall();
	__asm__("sti");
	Print("Interrupts Installed\n", GREEN);

	ScanPCI();
	PrintSepration();

	AC97Init();

	Print("AC97 Sound Card Initialized\n", GREEN);
	PrintSepration();

	_FPUInit();
	GenerateSineWave();
	AC97Play();

	Print("Finish!", BLUE);
	_Halt();
}
