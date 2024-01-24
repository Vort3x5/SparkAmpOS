#include <stdtypes.h>
#include <video.h>
#include <interrupts.h>
#include <memory.h>
#include <pci.h>
#include <audio.h>

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

	InitDMem();

	ScanPCI();

	Print("Finish!", WHITE);
}
