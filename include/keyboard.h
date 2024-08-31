#pragma once

enum Kb_Scancodes
{
	KEY_ESC = 0x01,
	KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9, KEY_0,
	KEY_BACKSPACE = 0x0e, KEY_TAB,
	KEY_Q, KEY_W, KEY_E, KEY_R, KEY_T, KEY_Y, KEY_U, KEY_I, KEY_O, KEY_P,
	KEY_ENTER = 0x1c, KEY_LCTRL,
	KEY_A, KEY_S, KEY_D, KEY_F, KEY_G, KEY_H, KEY_J, KEY_K, KEY_L,
	KEY_LSHIFT = 0x2a,
	KEY_Z = 0x2c, KEY_X, KEY_C, KEY_V, KEY_B, KEY_N, KEY_M,
	KEY_LALT = 0x38, KEY_SPACE
};

void KbHandlerInstall();
void KbHandler();
