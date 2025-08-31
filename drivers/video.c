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
		switch (*c)
		{
			case 'i':
			case 'd':
			    s64 num = va_arg(args, s64);
				if (num < 0)
				{
					PutC('-', color);
					num = -num;
				}
			    PrintStr(NumToStr(num, 10).data, color);
				break;

			case 'u':
			    u64 num = va_arg(args, u64);
			    PrintStr(NumToStr(num, 10).data, color);
				break;

			case 'x':
			    u64 num = va_arg(args, u64);
			    PrintStr(NumToStr(num, 16).data, color);
				break;

			case 'b':
			    u64 num = va_arg(args, u64);
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

void PrintNum(u64 num, enum Colors color)
{
	if (num == 0)
	{
		PutC('0', color);
		return;
	}
	u64 buff = num;
 	u16 len = 0;
	while (buff)
	{
		++len;
		buff /= 10;
	}

	char *backward = ALLOC_ARRAY(&g_frame_buffer, char, len); 
	char *msg = ALLOC_ARRAY(&g_frame_buffer, char, len + 1);
	for (s32 i = 0; i < len; ++i, num /= 10)
		backward[i] = '0' + (num % 10);

	for (s32 i = 0, j = len - 1; i < len; ++i, --j)
		msg[i] = backward[j];
	msg[len] = '\0';
	Print(msg, color);
}

void PrintSepration()
{
	for (s32 i = 0; i < TEXT_MODE_COLS; ++i)
		PutC('-', DARK_GRAY);
}

void PrintIter(u64 num, enum Colors color)
{
	PrintNum(num, color);
	if (num == 0)
	{
		--(tty.addr);
		return;
	}
	u8 res;
	for (res = 0; num; num /= 10)
		++res;
	tty.addr -= res;
}
