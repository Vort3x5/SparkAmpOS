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

u8 *Malloc(u64 size)
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

Arena_Region *NewArenaRegion(Arena *arena, u64 size)
{
	u64 total_size = sizeof(Arena_Region) + size;

	Arena_Region *region = (Arena_Region *)Malloc(total_size);;
	region->next = NULL;
	region->size = size;
	region->used = 0;
	MemSet(region->data, 0, size);

	return region;
}

void *ArenaAlloc(Arena *arena, u64 size, u64 alignment)
{
	if (size == 0)
		FAILED("ERROR: Nothing to allocate!");

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

void *ArenaRealloc(Arena *arena, void *prev_ptr, u64 prev_size, u64 new_size, u64 alignment)
{
	if (prev_ptr == NULL)
		return ArenaAlloc(arena, new_size, alignment);

	if (new_size == 0)
		return NULL;

	u8 *region_end = arena->end->data + arena->end->used;
	u8 *expected_end = (u8 *)prev_ptr + prev_size;

	if (expected_end == region_end)
	{
		if (new_size <= prev_size)
		{
			arena->end->used -= (prev_size - new_size);
			return prev_ptr;
		}
		else
		{
			u64 add_bytes = new_size - prev_size;
			if (arena->end->used + add_bytes <= arena->end->size)
			{
				arena->end->used += add_bytes;
				return prev_ptr;
			}
		}
	}

	void *new_ptr = ArenaAlloc(arena, new_size, alignment);
	u64 copy_size = (prev_size < new_size) ? prev_size : new_size;
	MemCpy(prev_ptr, new_ptr, copy_size);
	return new_ptr;
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

void MemSet(void *src, s32 value, u64 size)
{
	u8 *ptr = src;
	for (u64 i = 0; i < size; ++i)
		ptr[i] = value;
}

void MemCpy(void *src, void *dest, u64 size)
{
	u8 *from = src, *to = dest;
	for (u64 i = 0; i < size; ++i)
		to[i] = from[i];
}

void MemDump()
{
}
