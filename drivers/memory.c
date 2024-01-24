#include <stdint.h>
#define MEM_DEF
#include <memory.h>

#include <stdtypes.h>
#include <video.h>

u32 mmap_size;
struct MemMapEntry *mmap;
void *curr_addr;
u64 curr_entry;

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
	curr_addr = (void *)mmap->base;
	curr_entry = 0;
}

void *Malloc(u64 len)
{
	void *end_addr = (void *)(mmap[curr_entry].base + mmap[curr_entry].len);
	if (end_addr - curr_addr >= len)
	{
		void *res = curr_addr;
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
		curr_addr = (void *)(mmap[curr_entry].base + len);
		return (void *)mmap[curr_entry].base;
	}
	Print("ERROR: No Memory Free\n", RED);
	__asm__("hlt");
	return 0;
}

void *AlignedMalloc(u64 len, u64 alignment)
{
	void *buffer = Malloc(len + alignment - 1);
	uintptr_t aligned_addr = ((uintptr_t)buffer + alignment - 1) & ~(alignment - 1);

	curr_addr = (void *)aligned_addr;
	return (void *)aligned_addr;
}
