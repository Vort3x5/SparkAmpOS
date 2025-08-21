#pragma once

#include <stdtypes.h>

#define AllocType(arena, type) \
    ((type *))Alloc((arena), sizeof(type)))

#define AllocArray(arena, type, count) \
    ((type *)Alloc((arena), sizeof(type) * (count)))

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

#ifdef MEM_DEF

static u8 temp_buffer[128 * 1024];
static u8 video_buffer[64 * 1024];
static u8 temp_buffer[512 * 1024];

Arena temp_arena, video_arena, audio_arena;

#endif

extern Arena temp_arena;
extern Arena video_arena;
extern Arena audio_arena;

static inline u64 AlignUp(u64 value, u64 alignment)
    { return (value + alignment - 1) & ~(alignment - 1); }

void InitDMem();

void ArenaInit(Arena *arena, void *buffer, u64 size);
void *Alloc(Arena *arena, u64 size);
void *AllignedAlloc(Arena *arena, u64 size, u64 alignment);
void Free(Arena *arena);

void Memset(void *src, s32 value, s32 size);
void MemDump();
