#define KB_DEF
#include <keyboard.h>

#include <stdtypes.h>
#include <interrupts.h>
#include <io.h>
#include <video.h>
#include <demo.h>

void KbHandlerInstall()
{
	InstallIRQHandler(KbHandler, 1);
	k_pressed = false;
}

void KbHandler()
{
	k_pressed = true;
	u8 scancode = InB(0x60);
	PrintNum(scancode, MAGENTA);
}
