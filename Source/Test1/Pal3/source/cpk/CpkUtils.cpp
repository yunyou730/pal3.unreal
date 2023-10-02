#include "../../headers/cpk/CpkUtils.h"
#include <cstdarg>
#include <cstdio>

#include <string>
#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <cstring>
#include <algorithm>
#include <cctype>
#include <filesystem>

namespace pal3
{
	namespace fs = std::filesystem;

	const std::string CpkUtils::PATH_SEQPARATOR = "\\";

	void CpkUtils::Printf(const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		vprintf(format, args);
		va_end(args);
	}

	bool CpkUtils::endsWithPathSeparator(const std::string& str)
	{
		fs::path filePath(str);
		return filePath.string().back() == PATH_SEQPARATOR.back();
	}

	bool CpkUtils::CreateDirectory(const std::string& path)
	{
		//printf("[CreateDirectory] %s\n",path.c_str());
		try
		{
			if (!std::filesystem::exists(path))
			{
				std::filesystem::create_directory(path);
			}
			return true;
		}
		catch (const std::exception& ex)
		{
			printf("Create direction failed %s\n", ex.what());
			return false;
		}
	}
}
