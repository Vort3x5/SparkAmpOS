#include <stdtypes.h>
#include <video.h>
#include <interrupts.h>
#include <clock.h>
#include <memory.h>
#include <pci.h>
#include <hda.h>

void Main() 
{
	tty = &(TTY) {
		.vga_text_buffer = (word *) VGA_ADDR,
		.addr = 0
	};
	Clear();

	Print("Start!\n", WHITE);
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

    HDAInit();
    HDAIdentifyCodecs();
    HDAConfigCodec();
	_Halt();

	HDASendCommand(0x000f0004);
	Print("HDA Command Sent!\n", GREEN);
	u32 response = HDAReadResponse();

    u32 audio_buff = Malloc(AUDIO_SAMPLE_SIZE);
    LoadAudioData(audio_buff);
    HDAConfigOutStream(audio_buff, AUDIO_SAMPLE_SIZE);
    StartAudioPlayback();

	Print("Finish!", WHITE);
	_Halt();
}
