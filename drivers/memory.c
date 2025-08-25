#define MEM_DEF
#include <memory.h>

#include <utils.h>
#include <stdtypes.h>
#include <video.h>
#include <interrupts.h>

void InitDMem()
{
	mmap_count = *((u32 *)0x2000);
	mmap = (struct MemMapEntry *)0x2004;

	u64 kernel_end_addr = (u64)&_kernel_end;
	next_alloc_base = AlignUp(kernel_end_addr + 0x1000, 4096);
} 

u8 *Malloc(u64 size )
{
	size = AlignUp(size, 64);

	for (u32 i = 0; i < mmap_count; ++i)
	{
		if (!(mmap[i].acpi & 1) || mmap[i].type != 1)
			continue;

		u64 region_start = mmap[i].base;
		u64 region_end = mmap[i].base + mmap[i].len;

		if (next_alloc_base >= region_start && next_alloc_base + size <= region_end)
		{
			u8 *result = (u8 *)next_alloc_base;
			next_alloc_base += size;
			return result;
		}
		else if (next_alloc_base < region_start && region_start + size <= region_end)
		{
			next_alloc_base = region_start;
			u8 *result = (u8 *)next_alloc_base;
			next_alloc_base += size;
			return result;
		}
	}
	FAILED("ERROR: No more memory avaliable to allocate");
}

void *ArenaAlloc(Arena *arena, u64 size, u64 alignment)
{
	if (size == 0)
		return NULL;

	size = AlignUp(size, alignment);

	if (!arena->end || arena->end->used + size > arena->end->size)
	{
		Arena_Region *new_region = NewArenaRegion(arena, size);
		if (!new_region)
			FAILED("ERROR: Failed to create new region!");

		if (!arena->begin)
			arena->begin = new_region;
		else
		{
			Arena_Region *last = arena->begin;
			while (last->next)
				last = last->next;
			last->next = new_region;
		}
		arena->end = new_region;
	}

	u64 aligned_offset = AlignUp(arena->end->used, alignment);
	if (aligned_offset + size > arena->end->size)
		FAILED("ERROR: Region too small after alignment!");

	void *result = arena->end->data + aligned_offset;
	arena->end->used = aligned_offset + size;
	return result;
}

Arena_Region *NewArenaRegion(Arena *arena, u64 size)
{
	u64 total_size = sizeof(Arena_Region) + size;

	Arena_Region *region = (Arena_Region *)Malloc(total_size);;
	region->next = NULL;
	region->size = size;
	region->used = 0;
	Memset(region->data, 0, size);

	return region;
}

void Free(Arena *arena)
{
	for (Arena_Region *r = arena->begin; r != NULL; r = r->next)
		r->used = 0;
	arena->end = arena->begin;
}

Arena_Mark ArenaSnapshot(Arena *arena)
{
	Arena_Mark mark;
	if (arena->end == NULL)
		mark = (Arena_Mark) {
			.region = NULL,
			.used = 0,
		};
	else
		mark = (Arena_Mark) {
			.region = arena->end,
			.used = arena->end->used,
		};
	return mark;
}

void ArenaRewind(Arena *arena, Arena_Mark mark)
{
	if (mark.region == NULL)
	{
		Free(arena);
		return;
	}

	mark.region->used = mark.used;
	for (Arena_Region *r = mark.region->next; r != NULL; r = r->next)
		r->used = 0;
	arena->end = mark.region;
}

void Memset(void *src, s32 value, s32 size)
{
	u8 *ptr = src;
	for (s32 i = 0; i < size; ++i)
		ptr[i] = value;
}

void MemDump()
{
	Print("Num of entries: ", WHITE);
	PrintNum(mmap_count, LIGHT_CYAN);
	PutC('\n', WHITE);
	Print("(Base | Len | Type): ", WHITE);
	PutC('\n', WHITE);
	for (s32 i = 0; i < mmap_count; ++i)
	{
		PrintNum(mmap[i].base, LIGHT_CYAN);
		Print(" | ", WHITE);
		PrintNum(mmap[i].len, LIGHT_CYAN);
		Print(" | ", WHITE);
		PrintNum(mmap[i].type, LIGHT_CYAN);
		PutC('\n', WHITE);
	}
}
