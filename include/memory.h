#pragma once

#include <stdtypes.h>

#define ArenaAllocType(arena, type) \
    ((type *))ArenaAlloc((arena), sizeof(type)))

#define ArenaAllocArray(arena, type, count) \
    ((type *)ArenaAlloc((arena), sizeof(type) * (count)))

struct MemMapEntry
{
	u64 base;
	u64 len;
	u32 type;
	u32 acpi;
}__attribute__((packed));

typedef struct ArenaRegion ArenaRegion;
typedef struct Arena Arena;

struct ArenaRegion
{
	u64 size, used;
	u8 data[];
};

struct Arena
{
	ArenaRegion *region;
};

static inline u64 AlignUp(u64 value, u64 alignment)
    { return (value + alignment - 1) & ~(alignment - 1); }

void Memset(void *src, s32 value, s32 size);

void InitDMem();
u64 Malloc(u64 len);
u64 AlignedMalloc(u64 len, u64 alignment);
void Free(u64 addr, u64 len);

void ArenaInit(Arena *arena, void *buffer, u64 size);
void *ArenaAlloc(Arena *arena, u64 size);
void ArenaReset(Arena *arena);
void ArenaFree(Arena *arena);

void MemDump();
