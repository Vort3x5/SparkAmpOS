#include <video.h>

#include <stdtypes.h>
#include <io.h>

void Clear()
{
	byte *vga = (byte *) VGA_ADDR;
	s32 area = TEXT_MODE_COLS * TEXT_MODE_ROWS;

	for (s32 bt = 0; bt < area; ++bt)
	{
		*(vga++) = 0;
		*(vga++) = WHITE;
	}
}
