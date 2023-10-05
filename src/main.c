#include <stdtypes.h>
#include <video.h>
#include <audio.h>
#include <irq.h>

void Main() 
{
	TTY *tty = &(TTY) {
		.vga_text_buffer = (word *) VGA_ADDR, 
		.addr = 0
	};

	Clear(tty);
	Print(tty, "Start\nEnd", WHITE);

	IDTInstall();
	ISRsInstall();

	Clear(tty);
	s32 x = 5 / 0;
}
