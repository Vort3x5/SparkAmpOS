#define TTY_DEF
#include <video.h>

#include <stdtypes.h>
#include <memory.h>

void TTYReset()
{
	tty.vga_text_buffer = (u16 *) VGA_ADDR;
	tty.addr = 0;
}

void Clear()
{
	s32 area = TEXT_MODE_COLS * TEXT_MODE_ROWS;

	for (s32 i = 0; i < area; ++i)
		tty.vga_text_buffer[i] = VgaEntry(0, BLACK);

	TTYReset();
}

void PutC(char c, enum Colors color)
{
	switch(c)
	{
		case '\n':
			tty.addr += TEXT_MODE_COLS - ((tty.addr) % TEXT_MODE_COLS);
			break;

		default:
			tty.vga_text_buffer[tty.addr] = VgaEntry(c, color);
			++(tty.addr);
			break;
	}
}

void Print(enum Colors color, const char *msg, ...)
{
	va_list args;
	va_start(args, msg);

	for (const char *c = msg; *c; ++c)
	{
		if (*c != '%')
		{
			PutC(*c, color);
			continue;
		}

		++c;
		s64 snum;
		u64 num;
		switch (*c)
		{
			case 'i':
			case 'd':
			    snum = va_arg(args, s64);
				if (num < 0)
				{
					PutC('-', color);
					snum = -snum;
				}
			    PrintStr(NumToStr(snum, 10).data, color);
				break;

			case 'u':
			    num = va_arg(args, u64);
			    PrintStr(NumToStr(num, 10).data, color);
				break;

			case 'x':
			    num = va_arg(args, u64);
			    PrintStr(NumToStr(num, 16).data, color);
				break;

			case 'b':
			    num = va_arg(args, u64);
			    PrintStr(NumToStr(num, 2).data, color);
				break;

			case 'c':
				PutC((char)va_arg(args, char), color);
				break;

			case 's':
				PrintStr(va_arg(args, char *), color);
				break;

			case '%':
				PutC('%', color);
				break;

			default:
				PutC('%', color);
				PutC(*c, color);
				break;
		}
	}

	va_end(args);
}

void PrintStr(const char *str, enum Colors color)
{
	for (s32 i = 0; str[i]; ++i)
		PutC(str[i], color);
}

void PrintSepration()
{
	for (s32 i = 0; i < TEXT_MODE_COLS; ++i)
		PutC('-', DARK_GRAY);
}
