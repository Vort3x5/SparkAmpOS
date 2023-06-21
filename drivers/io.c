#include <io.h>

#include <stdtypes.h>

byte InPortB(word port)
{
	byte res;
	__asm__("in %%dx, %%al" : "=a" (res) : "d" (port));
	return res;
}

void OutPortB(word port, byte data)
{
	__asm__("out %%al, %%dx" : "=a" (data) : "d" (port));
}
