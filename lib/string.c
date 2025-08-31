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

String FormatToString(const char *str)
{
	if (str[0] != '%')
		FAILED("ERROR: Nothing to format");

	switch (str[1])
	{
		case 'd':
		    break;

		case 'x':
		    break;

		case '%':
		case 'c':
		    break;

		case 's':
		    break;

		default:
			FAILED("ERROR: Wrong format specifier!");
		    break;
	}
}

String NumToStr(u64 num)
{
}
