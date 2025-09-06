#pragma once

#include <stdtypes.h>

#define VGA_ADDR 0xb8000

#define TEXT_MODE_ROWS 25
#define TEXT_MODE_COLS 80

#define VGA_CTRL_REG 0x3d4
#define VGA_DATA_REG 0x3d5

enum Colors
{
	BLACK = 0,
	BLUE,
	GREEN,
	CYAN,
	RED,
	MAGENTA,
	BROWN,
	LIGHT_GRAY,
	DARK_GRAY,
	LIGHT_BLUE,
	LIGHT_GREEN,
	LIGHT_CYAN,
	LIGHT_RED,
	LIGHT_MAGENTA,
	YELLOW,
	WHITE
};

typedef struct {
	u16 *vga_text_buffer;
	u32 addr;
} TTY;

typedef struct {
	bool t_long, t_long_long, 
	    uppercase, zero_pad, left_align;
	int width, precision;
} FormatSpecifier;

#ifdef TTY_DEF

static TTY tty;

#endif

static inline u16 VgaEntry(u8 c, enum Colors color)
	{ return (u16) c | (u16) color << 8; }

void TTYReset();
void Clear();
void PutC(char c, enum Colors color);
void PrintStr(const char *msg, enum Colors color);
FormatSpecifier ParseFormatSpec(const char **format_ptr);
void Print(enum Colors color, const char *msg, ...);
void PrintSepration();
