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

String FormatToString(const char *str, void *arg)
{
	if (str[0] != '%')
		FAILED("ERROR: Nothing to format");

	u64 num = 0;
	switch (str[1])
	{
		case 'b':
			num = (u64)arg;
		    return NumToStr(num, 2);

		case 'd':
			num = (u64)arg;
		    return NumToStr(num, 10);

		case 'x':
			num = (u64)arg;
		    return NumToStr(num, 16);

		case '%':
			return StringFrom("%");
			break;

		case 'c':
		case 's':
			return StringFrom((const char *)arg);
		    break;

		default:
			FAILED("ERROR: Wrong format specifier!");
		    break;
	}
}

String NumToStr(u64 num, u8 num_sys)
{
	u8 digits = 0;
	for (u64 i = num; i; i /= num_sys)
		++digits;

	String str = (String) {
		.data = ALLOC_ARRAY(&g_frame_buffer, char, digits + 1),
	    .len = digits + 1,
	};
	for (u32 i = digits; num; num /= num_sys, --i)
		str.data[i] = num % num_sys;
	return str;
}
