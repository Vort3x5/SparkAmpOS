#define TTY_DEF
#include <video.h>

#include <stdtypes.h>

TTY *tty;

void Clear()
{
	s32 area = TEXT_MODE_COLS * TEXT_MODE_ROWS;

	for (s32 i = 0; i < area; ++i)
		tty->vga_text_buffer[i] = VgaEntry(0, WHITE);

	tty->vga_text_buffer = (word *) VGA_ADDR;
	tty->addr = 0;
}

void PutC(char c, enum Colors color)
{
	switch(c)
	{
		case '\n':
			tty->addr += TEXT_MODE_COLS - ((tty->addr) % TEXT_MODE_COLS);
			break;

		default:
			tty->vga_text_buffer[tty->addr] = VgaEntry(c, color);
			++(tty->addr);
			break;
	}
}

void Print(const char *msg, enum Colors color)
{
	for (s32 i = 0; msg[i]; ++i)
		PutC(msg[i], color);
}

void PrintNum(u64 num)
{
}
