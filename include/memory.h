#pragma once

#include <stdtypes.h>

struct MemMapEntry
{
	u64 base;
	u64 len;
	u32 type;
	u32 acpi;
}__attribute__((packed));

#ifdef MEM_DEF

static u32 mmap_size;
static struct MemMapEntry *mmap;
static u64 curr_addr;
static u64 curr_entry = 0;

#endif

void Memset(void *src, s32 value, s32 size);

void InitDMem();
u64 Malloc(u64 len);
u64 AlignedMalloc(u64 len, u64 alignment);
void Free(u64 addr, u64 len);
