#define KB_DEF
#include <keyboard.h>

#include <stdtypes.h>
#include <interrupts.h>
#include <io.h>
#include <video.h>

void KbHandlerInstall()
{
	InstallIRQHandler(KbHandler, 1);
}

void KbHandler()
{
	byte scancode = InB(0x60);
	PutC(scancode, MAGENTA);
	_Halt();
}
