#include <stdtypes.h>
#include <video.h>
#include <interrupts.h>
#include <pci.h>

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

	ScanPCI();

	Print("Finish!", WHITE);
}
