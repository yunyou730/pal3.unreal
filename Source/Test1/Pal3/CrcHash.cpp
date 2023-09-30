#include "CrcHash.h"
#include <sstream>
#include <codecvt>
#include <vector>
#include <string>
#include <locale>

namespace pal3
{
	std::wstring ConvertToWideString(const std::string& narrowString) {
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		std::wstring wideString = converter.from_bytes(narrowString);

		return wideString;
	}

	std::string ConvertToGB2312(const std::wstring& wideString) {
		std::wstring_convert<std::codecvt_byname<wchar_t, char, std::mbstate_t>> converter(new std::codecvt_byname<wchar_t, char, std::mbstate_t>("zh_CN.GB2312"));
		std::string gb2312String = converter.to_bytes(wideString);

		return gb2312String;
	}

	std::string ConvertStringToGB2312(const std::string& inputString) {
		std::wstring wideString = ConvertToWideString(inputString);
		std::string gb2312String = ConvertToGB2312(wideString);

		return gb2312String;
	}

	void Crc32Hash::Init()
	{
		for (uint32_t i = 0;i <= CRC_TABLE_MAX;i++)
		{
			uint32_t crcAccum = i << 24;
			for (uint8_t j = 0;j < 8;j++)
			{
				crcAccum = (crcAccum & 0x80000000L) != 0 ? (crcAccum << 1) ^ POLYNOMIAL : (crcAccum << 1);
			}
			_crcTable[i] = crcAccum;
		}
		_hasInitialzied = true;
	}

	uint32_t Crc32Hash::Compute(const std::string& str, int codePage, bool bUseCache)
	{
		std::string cacheKey = str + std::to_string(codePage);
		
		if (_crcResultCache.find(cacheKey) != _crcResultCache.end())
		{
			return _crcResultCache[cacheKey];
		}

		// str to binary stream
		// Here we need to get string GB2312 binary data ,which codePage = 936 
		//std::vector<uint8_t> binaryData(str.begin(), str.end());

		std::string gb2312Str = ConvertStringToGB2312(str);
		std::vector<uint8_t> binaryData(gb2312Str.begin(), gb2312Str.end());
		

		uint32_t crc = ComputeInternal(binaryData);
		_crcResultCache.insert(std::make_pair(cacheKey, crc));

		return crc;
	}

	uint32_t Crc32Hash::ComputeInternal(const std::vector<uint8_t>& binData)
	{
		if (binData.size() == 0)
		{
			return 0;
		}

		int idx = 0;

		uint32_t result = (uint32_t)binData[idx] << 24;
		idx++;
		
		for (int i = 1;i < 4 && i < binData.size();i++)
		{
			if (binData[idx] == 0)
			{
				break;
			}
			result |= (uint32_t)(binData[idx] << (24 - 8 * i));
			idx++;
		}

		result = ~result;

		for (int i = 4;i < binData.size() && binData[idx] != 0;i++,idx++)
		{
			result = (result << 8 | binData[idx]) ^ _crcTable[result >> 24];
		}

		return ~result;
	}
}