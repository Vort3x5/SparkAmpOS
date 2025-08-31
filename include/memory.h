#pragma once

#include <stdtypes.h>

#define NORESET_REGION_SIZE (128 * 1024)
#define TEMP_REGION_SIZE (256 * 1024)
#define FRAME_REGION_SIZE (64 * 1024)
#define ARRAY_INIT_CAP 256

#define ALLOC(arena, type) \
    ((type *))ArenaAlloc((arena), sizeof(type), _Alignof(type))

#define ALLOC_ARRAY(arena, type, count) \
    ((type *)ArenaAlloc((arena), sizeof(type) * (count), _Alignof(type)))

#define REALLOC(arena, prev_ptr, type, prev_size, new_size) \
    ((type *))ArenaRealloc((arena), (prev_ptr), (prev_size), (new_size), _Alignof(type))

#define REALLOC_ARRAY(arena, prev_ptr, type, prev_count, new_count) \
    ((type *))ArenaRealloc((arena), (prev_ptr),                     \
						  sizeof(type) * (prev_count),              \
                          sizeof(type) * (new_count),               \
						   _Alignof(type))

#define ARRAY_PUSH(arena, arr, item)                                          \
    do {                                                                      \
	    if ((arr)->used >= (arr)->size) {                                     \
	        u64 new_size = (arr)->size == 0 ? ARRAY_INIT_CAP : (arr)->size*2; \
	        (arr)->data = REALLOC_ARRAY((arena), (arr)->data,                 \
			                            __typeof__((arr)->data[0]),           \
			                            (arr)->used, (arr)->size);            \
	    }                                                                     \
	    (arr)->data[(arr)->used++] = (item);                                  \
	} while (0)

struct MemMapEntry
{
	u64 base, len;
	u32 type, acpi;
}__attribute__((packed));

typedef struct Arena_Region Arena_Region;
typedef struct Arena Arena;
typedef struct Arena_Mark Arena_Mark;

struct Arena_Region
{
	Arena_Region *next;
	u64 size, used;
	u8 data[];
};

struct Arena 
{
	Arena_Region *begin, *end;
};

struct Arena_Mark 
{
	Arena_Region *region;
	u64 used;
};

typedef struct{
	u8 *data;
	u64 used, size;
} Array;

#ifdef MEM_DEF

static u32 mmap_count;
static struct MemMapEntry *mmap;
static u64 next_alloc_base;

Arena g_noreset_buffer, g_temp_buffer, g_frame_buffer;

extern u8 _kernel_end;

#endif

extern Arena g_noreset_buffer, g_temp_buffer, g_frame_buffer;

static inline u64 AlignUp(u64 value, u64 alignment)
    { return (value + alignment - 1) & ~(alignment - 1); }

void InitDMem();
u8 *Malloc(u64 size);

Arena_Region *NewArenaRegion(Arena *arena, u64 size);
void *ArenaAlloc(Arena *arena, u64 size, u64 alignment);
void *ArenaRealloc(Arena *arena, void *prev_ptr, u64 prev_size, u64 new_size, u64 alignment);
void Free(Arena *arena);

Arena_Mark ArenaSnapshot(Arena *arena);
void ArenaRewind(Arena *arena, Arena_Mark mark);

void MemSet(void *src, s32 value, u64 size);
void MemCpy(void *src, void *dest, u64 size);
void MemDump();
