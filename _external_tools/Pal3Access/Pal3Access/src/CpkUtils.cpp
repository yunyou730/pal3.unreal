#include "../headers/CpkUtils.h"
#include <cstdarg>
#include <cstdio>

namespace pal3
{
	void CpkUtils::Printf(const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		vprintf(format, args);
		va_end(args);
	}
}
