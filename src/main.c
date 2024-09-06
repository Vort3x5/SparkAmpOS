#include <stdtypes.h>
#include <video.h>
#include <interrupts.h>
#include <clock.h>
#include <keyboard.h>
#include <memory.h>
#include <pci.h>
#include <ac97.h>

#include <demo.h>

void Main() 
{
	TTYReset();

	Clear();
	Print("Start!\n", BLUE);

	InitDMem();
	Print("Dynamic Memory Initialized\n", GREEN);

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

	// MemDump();
	// PrintSepration();
	GenerateSineWave();
	AC97Play();

	Brk();

	Print("Finish!", BLUE);
	_Halt();
}
