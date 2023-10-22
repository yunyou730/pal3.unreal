#include "../../headers/encoding/Encoding.h"

namespace pal3
{
	std::wstring Encoding::ConvertToWideString(const std::string& narrowString)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		std::wstring wideString = converter.from_bytes(narrowString);

		return wideString;
	}

	std::string Encoding::ConvertToGB2312(const std::wstring& wideString)
	{
		std::wstring_convert<std::codecvt_byname<wchar_t, char, std::mbstate_t>> converter(new std::codecvt_byname<wchar_t, char, std::mbstate_t>("zh_CN.GB2312"));
		std::string gb2312String = converter.to_bytes(wideString);

		return gb2312String;
	}

	std::string Encoding::ConvertStringToGB2312(const std::string& inputString)
	{
		std::wstring wideString = ConvertToWideString(inputString);
		std::string gb2312String = ConvertToGB2312(wideString);

		return gb2312String;
	}
}
