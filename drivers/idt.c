#include <pic.h>

#include <stdtypes.h>
#include <memory.h>

struct IDT_Entry idt[256];
struct IDT_Ptr idtp;

void IDTSetGate(byte index, dword base, word sel, byte flags)
{
	idt[index] = (struct IDT_Entry) {
		.base_lo = (base) & 0xffff,
		.base_hi = (base >> 16) & 0xffff,
		.sel = sel,
		.flags = flags | 0x60,
		.always0 = 0
	};
}

void IDTInstall()
{
	idtp.limit = (sizeof (struct IDT_Entry) * 256) - 1;
	idtp.base = (dword) &idt[0];

	// Memset(&idt, 0, sizeof (struct IDT_Entry) * 256);

	_IDTLoad();
}
