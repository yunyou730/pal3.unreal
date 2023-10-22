#pragma once

#include <cstdint>
#include <string>
#include <map>
#include <vector>

namespace pal3
{
	class Crc32Hash
	{
	public:
		void Init();
		uint32_t Compute(const std::string& str,int codePage,bool bUseCache = true);
	private:
		uint32_t ComputeInternal(const std::vector<uint8_t>& binData);
		

	private:
		static const uint32_t CRC_TABLE_MAX = 0xff;
		static const uint32_t POLYNOMIAL = 0x04c11db7;

		uint32_t _crcTable[CRC_TABLE_MAX + 1];
		bool _hasInitialzied = false;

		// Cache the crc32 hash result for string since its expensive to compute
		std::map<std::string, uint32_t> _crcResultCache;
	};
}
