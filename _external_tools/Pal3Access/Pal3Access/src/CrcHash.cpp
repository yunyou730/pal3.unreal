#include "../headers/CrcHash.h"
#include "../headers/encoding/Encoding.h"
#include <sstream>
#include <codecvt>
#include <vector>
#include <string>
#include <locale>
#include <cassert>

namespace pal3
{
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

		//std::string gb2312Str = Encoding::ConvertStringToGB2312(str);
		std::string gb2312Str = str;
		std::vector<uint8_t> binaryData(gb2312Str.begin(), gb2312Str.end());
		

		uint32_t crc = ComputeInternal(binaryData);
		_crcResultCache.insert(std::make_pair(cacheKey, crc));

		return crc;
	}

	uint32_t Crc32Hash::ComputeInternal(const std::vector<uint8_t>& binData)
	{
		assert(_hasInitialzied,"Not initialized Crc32Hash");

		if (binData.size() == 0)
		{
			return 0;
		}

		int idx = 0;

		uint32_t result = (uint32_t)binData[idx] << 24;
		idx++;
		
		for (size_t i = 1;i < 4 && i < binData.size();i++)
		{
			if (binData[idx] == 0)
			{
				break;
			}
			result |= (uint32_t)(binData[idx] << (24 - 8 * i));
			idx++;
		}

		result = ~result;

		for (size_t i = 4;i < binData.size() && binData[idx] != 0;i++,idx++)
		{
			result = (result << 8 | binData[idx]) ^ _crcTable[result >> 24];
		}

		return ~result;
	}
}