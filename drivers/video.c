#include <video.h>

#include <stdtypes.h>
#include <io.h>

void Clear(TTY *tty)
{
	s32 area = TEXT_MODE_COLS * TEXT_MODE_ROWS;

	for (s32 i = 0; i < area; ++i)
		tty->vga_text_buffer[i] = VgaEntry(0, WHITE);
	tty->addr = 0;
}

void PutC(TTY *tty, byte c, enum Colors color)
{
	switch(c)
	{
		case '\n':
			tty->addr += TEXT_MODE_COLS - ((tty->addr) % TEXT_MODE_COLS);
			break;

		default:
			tty->vga_text_buffer[tty->addr] = VgaEntry(c, color);
			++(tty->addr);
	}
}

void Print(TTY *tty, const char *msg, enum Colors color)
{
	for (s32 i = 0; msg[i]; ++i)
		PutC(tty, msg[i], color);
}
