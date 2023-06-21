#include <stdtypes.h>

void _main() 
{
	byte *vga = (byte *) 0xb8000;
	*(vga + 1) = 'X';
	*(vga + 2) = 0x0f;
	while (1) {
	}
}
