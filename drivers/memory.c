#define MEM_DEF
#include <memory.h>

#include <stdtypes.h>
#include <video.h>
#include <interrupts.h>
#include <demo.h>

static u32 mmap_size;
static struct MemMapEntry mmap[32];
static u64 curr_addr;
static u64 curr_entry = 0;

void Memset(void *src, s32 value, s32 size)
{
	byte *ptr = src;
	for (s32 i = 0; i < size; ++i)
		ptr[i] = value;
}

void InitDMem()
{
	mmap_size = *((u32 *)0x2000);
	// assigning 0x2004 address to MemMapEntry struct doesn't work with standard gcc 
	u32 *raw_mem_data = (u32 *)0x2004;
	for (u32 i = 0; i < mmap_size; ++i)
	{
		u32 offset = (i * 6);

		u32 base_low = raw_mem_data[offset];
		u32 base_high = raw_mem_data[offset + 1];
		u32 len_low = raw_mem_data[offset + 2];
		u32 len_high = raw_mem_data[offset + 3];
		u32 type = raw_mem_data[offset + 4];
		u32 acpi = raw_mem_data[offset + 5];

		mmap[i] = (struct MemMapEntry) {
			.base = (u64)(base_high << 32) | base_low,
			.len = (u64)(len_high << 32) | len_low,
			.type = type,
			.acpi = acpi,
		};
	}
}

void ArenaInit(Arena *arena, void *buffer, u64 size)
{
	arena->region = (ArenaRegion *)buffer;
	arena->region->size = size - sizeof(ArenaRegion);
	arena->region->used = 0;
}

void *Alloc(Arena *arena, u64 size)
{
	if (size == 0)
		return NULL;

	size = AlignUp(size, 8);

	if (arena->region->used + size > arena->region->size)
		return NULL;

	void *result = arena->region->data + arena->region->used;
	arena->region->used += size;
	return result;
}

void *AllignedAlloc(Arena *arena, u64 size, u64 alignment)
{
	if (size == 0)
		return NULL;

	u64 current_pos = (u64)(arena->region->data + arena->region->used);
	u64 aligned_pos = AlignUp(current_pos, alignment);
	u64 padding = aligned_pos - current_pos;

	if (arena->region->used + padding + size > arena->region->size)
		return NULL;

	arena->region->used += padding + size;
	return (void *)aligned_pos;
}

void ArenaReset(Arena *arena)
{
	arena->region->used = 0;
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
			_Halt();
		}
		curr_addr = (mmap[curr_entry].base + len);
		return mmap[curr_entry].base;
	}
	Print("ERROR: No Memory Free\n", RED);
	_Halt();
	return 0;
}

u64 AlignedMalloc(u64 len, u64 alignment)
{
	u64 buffer = (u64)Malloc(len + alignment - 1);
	u64 aligned_addr = ((u64)buffer + alignment - 1) & ~(alignment - 1);

	curr_addr = aligned_addr + len;
	return aligned_addr;
}

void Free(u64 addr, u64 len)
{
	Memset((void *)addr, 0, len);
	curr_addr = addr;
}

void MemDump()
{
	Print("Num of entries: ", WHITE);
	PrintNum(mmap_size, LIGHT_CYAN);
	PutC('\n', WHITE);
	Print("(Base | Len | Type): ", WHITE);
	PutC('\n', WHITE);
	for (s32 i = 0; i < mmap_size; ++i)
	{
		PrintNum(mmap[i].base, LIGHT_CYAN);
		Print(" | ", WHITE);
		PrintNum(mmap[i].len, LIGHT_CYAN);
		Print(" | ", WHITE);
		PrintNum(mmap[i].type, LIGHT_CYAN);
		PutC('\n', WHITE);
	}
}
