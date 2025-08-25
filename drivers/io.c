#include <io.h>

#include <stdtypes.h>

u8 In8(u16 port)
{
	u8 res;
	__asm__("inb %1, %0" : "=a"(res) : "Nd"(port));
	return res;
}

u16 In16(u16 port)
{
	u16 res;
	__asm__("inw %1, %0" : "=a"(res) : "Nd"(port));
	return res;
}

u32 In32(u16 port)
{
	u32 res;
	__asm__("inl %1, %0" : "=a"(res) : "Nd"(port));
	return res;
}

void Out8(u16 port, u8 data)
{
	__asm__("outb %0, %1" : : "a"(data), "Nd"(port));
}

void Out16(u16 port, u16 data)
{
	__asm__("outw %0, %1" : : "a"(data), "Nd"(port));
}

void Out32(u16 port, u32 data)
{
	__asm__("outl %0, %1" : : "a"(data), "Nd"(port));
}

u8 MMIn8(u32 addr)
{
	return *((u8 *)(addr));
}

u16 MMIn16(u32 addr)
{
	return *((u16 *)(addr));
}

u32 MMIn32(u32 addr)
{
	return *((u32 *)(addr));
}

void MMOut8(u32 addr, u8 val)
{
	*((u8 *)(addr)) = val;
}

void MMOut16(u32 addr, u16 val)
{
	*((u16 *)(addr)) = val;
}

void MMOut32(u32 addr, u32 val)
{
	*((u32 *)(addr)) = val;
}
