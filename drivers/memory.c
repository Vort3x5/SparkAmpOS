#define MEM_DEF
#include <memory.h>

#include <stdtypes.h>
#include <video.h>

u32 mmap_size;
struct MemMapEntry *mmap;
u64 curr_addr;
u64 curr_entry = 0;

void Memset(void *src, s32 value, s32 size)
{
	byte *ptr = src;

	for (s32 i = 0; i < size; ++i)
		ptr[i] = value;
}

void InitDMem()
{
	mmap_size = *((u32 *)0x2000);
	mmap = (struct MemMapEntry *)0x2004;
	curr_addr = mmap->base;
}

u64 Malloc(u64 len)
{
	u64 end_addr = (mmap[curr_entry].base + mmap[curr_entry].len);
	if (end_addr - curr_addr >= len)
	{
		u64 res = curr_addr;
		curr_addr += len;
		return res;
	}
	else
	{
		++curr_entry;
		if (curr_entry >= mmap_size)
		{
			Print("ERROR: No Memory Free\n", RED);
			__asm__("hlt");
		}
		curr_addr = (mmap[curr_entry].base + len);
		return mmap[curr_entry].base;
	}
	Print("ERROR: No Memory Free\n", RED);
	__asm__("hlt");
	return 0;
}

u64 AlignedMalloc(u64 len, u64 alignment)
{
	u64 buffer = Malloc(len + alignment - 1);
	u64 aligned_addr = ((u64)buffer + alignment - 1) & ~(alignment - 1);

	curr_addr = aligned_addr;
	return aligned_addr;
}
