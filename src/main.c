#include <stdtypes.h>
#include <video.h>
#include <audio.h>
#include <pic.h>
#include <clock.h>

void Main() 
{
	tty = &(TTY) {
		.vga_text_buffer = (word *) VGA_ADDR,
		.addr = 0
	};

	Clear();

	Print("Start\nEnd\n", WHITE);

	IDTInstall();
	ISRsInstall();
	IRQsInstall();
	__asm__("sti");

	s32 x = 5 / 0;
	while (x);

	InstallTimer();

    // Sleep(2);
	Clear();
	Print("Finish", WHITE);
}
