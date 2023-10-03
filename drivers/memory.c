#include <memory.h>

#include <stdtypes.h>

void Memset(void *src, s32 value, s32 size)
{
	byte *ptr = src;

	for (s32 i = 0; i < size; ++i)
		*(ptr + i) = value;
}
