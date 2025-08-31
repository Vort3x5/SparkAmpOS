#pragma once

#define NULL 0

typedef char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef long s32;
typedef unsigned long u32;
typedef long long s64;
typedef unsigned long long u64;

typedef float f32;
typedef double f64;

typedef enum { false, true } bool;

typedef struct {
	char *data;
	u64 len;
} String;

typedef __builtin_va_list va_list;
#define va_start(ap, last) __builtin_va_start(ap, last)
#define va_end(ap) __builtin_va_end(ap)
#define va_arg(ap, type) __builtin_va_arg(ap, type)

String StringNew(u64 len);
String StringFrom(const char *c_str);

u64 StrLen(const char *str);
String FormatToString(const char *str, void *arg);
String NumToStr(u64 num, u8 num_sys);
String DecToHex(u64 num);
