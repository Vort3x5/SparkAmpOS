#include <stdtypes.h>
#include <video.h>
#include <audio.h>

void _main() 
{
	TTY *tty;

	tty->vga_text_buffer = (word *) VGA_ADDR;
	tty->addr = 0;

	Clear(tty);

	Print(tty, "Start\nEnd", WHITE);
	Beep();
	Clear(tty);
	Print(tty, "Finished", WHITE);
}
