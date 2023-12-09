#include <stdtypes.h>
#include <video.h>
#include <audio.h>
#include <pic.h>
#include <clock.h>

void Main() 
{
	TTY *tty = &(TTY) {
		.vga_text_buffer = (word *) VGA_ADDR,
		.addr = 0
	};

	Clear();

	Print("Start\nEnd\n", WHITE);

	IDTInstall();
	ISRsInstall();
	InstallIRQ();
	__asm__("sti");

	InstallTimer();

	// Sleep(2000);
	Clear();
	Print("Finish", WHITE);
}
