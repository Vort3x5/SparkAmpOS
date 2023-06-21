#pragma once

#include <stdtypes.h>

#define VGA_ADDR 0xb8000

#define TEXT_MODE_ROWS 25
#define TEXT_MODE_COLS 80

#define VGA_CTRL_REG 0x3d4
#define VGA_DATA_REG 0x3d5

typedef enum 
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
} Colors;

void Clear();
