#define STR_DEF
#include <stdtypes.h>

#include <utils.h>
#include <memory.h>

String StringFrom(const char *c_str)
{
	u64 len = StrLen(c_str);
	String str = StringNew(len);
	MemCpy(c_str, str.data, len);
	return str;
}

String StringNew(u64 len)
{
	String str = {
		.data = ALLOC_ARRAY(&g_frame_buffer, char, len + 1),
		.len = len + 1,
	};
	str.data[len] = '\0';
	return str;
}

u64 StrLen(const char *str)
{
	if (!str)
		return 0;

	u64 len = 0;
	while (str[len])
		++len;

	return len;
}

String NumToStr(u64 num, u8 sys)
{
	if (sys > 16 || sys < 2)
		FAILED("ERROR: Number system not supported!");

	u8 digits = 0;
	for (u64 i = num; i; i /= sys)
		++digits;

	String str = StringNew(digits + 1);
	for (u32 i = digits; num; num /= sys, --i)
		str.data[i] = nums_as_chars[num % sys];
	return str;
}
