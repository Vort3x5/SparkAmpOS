#pragma once

#include <stdtypes.h>

struct MemMapEntry
{
	u64 base;
	u64 len;
	u32 type;
	u32 acpi;
}__attribute__((packed));

#ifndef MEM_DEF

extern u32 mmap_size;
extern struct MemMapEntry *mmap;
extern u64 curr_addr;
extern u64 curr_entry;

#endif

void Memset(void *src, s32 value, s32 size);

void InitDMem();
u64 Malloc(u64 len);
u64 AlignedMalloc(u64 len, u64 alignment);
