#pragma once
#include <string>

namespace pal3
{
	class CpkUtils
	{

	public:
		static void Printf(const char* format,...);
		static bool endsWithPathSeparator(const std::string& str);
		static bool CreateDirectory(const std::string& path);

		static const std::string PATH_SEQPARATOR;
	};
}
