#include <stdtypes.h>
#include <video.h>
#include <interrupts.h>
#include <clock.h>
#include <memory.h>
#include <pci.h>
#include <hda.h>

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

 	// InitTimer(250);
	// Print("Timer Installed!\n", GREEN);
	__asm__("sti");
	Print("Interrupts Installed\n", GREEN);

	ScanPCI();
	PrintSepration();

    HDAInit();
    HDAConfigCodec();

	HDASendCommand(0x000f0004);
	Print("HDA Command Sent!\n", GREEN);
	u64 response = HDAReadResponse();
	Print("Response: ", WHITE);
	PrintNum(response, LIGHT_CYAN);
	_Halt();

    u32 audio_buff = Malloc(AUDIO_SAMPLE_SIZE);
    LoadAudioData(audio_buff);
    HDAConfigOutStream(audio_buff, AUDIO_SAMPLE_SIZE);
    StartAudioPlayback();

	Print("Finish!", BLUE);
	_Halt();
}
