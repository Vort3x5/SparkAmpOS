#include <stdtypes.h>
#include <video.h>
#include <audio.h>
#include <pic.h>

void Main() 
{
	TTY *tty = &(TTY) {
		.vga_text_buffer = (word *) VGA_ADDR,
		.addr = 0
	};

	Clear(tty);
	Print(tty, "Start\nEnd", WHITE);

	FHInitTTY(tty);
	IDTInstall();
	ISRsInstall();
	InstallIRQ();
}
