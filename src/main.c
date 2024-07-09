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

	IDTInstall();
	ISRsInstall();
	IRQsInstall();
	__asm__("sti");

	InstallTimer();
	Sleep(3000);

	Print("Finish!", WHITE);
	__asm__("cli");
	__asm__("hlt");
	InitDMem();

	ScanPCI();

    HDAInit();
    HDAIdentifyCodecs();
    HDAConfigCodec();

	// __asm__("cli");
	HDACodecCommand(0xf00, 4);
	u32 response = HDAReadResponse();
	PrintNum(response, GREEN);


    u32 audio_buff = Malloc(AUDIO_SAMPLE_SIZE);
    LoadAudioData(audio_buff);
    HDAConfigOutStream(audio_buff, AUDIO_SAMPLE_SIZE);
    StartAudioPlayback();
	// while (1);
	// __asm__("cli");
	// __asm__("hlt");

	Print("Finish!", WHITE);
}
