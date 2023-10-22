#pragma once
#include <sstream>
#include <codecvt>
#include <vector>
#include <string>
#include <locale>
#include <cassert>

namespace pal3
{
	class Encoding
	{
	public:
		static std::wstring ConvertToWideString(const std::string& narrowString);
		static std::string ConvertToGB2312(const std::wstring& wideString);
		static std::string ConvertStringToGB2312(const std::string& inputString);
	};
}
