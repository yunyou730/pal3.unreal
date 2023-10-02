#pragma once

#include<cstdint>

namespace pal3
{
	typedef enum
	{
		None = 0x0,
		IsValid = 0x1,
		IsDir = 0x2,
		IsLargeFile = 0x4,	// not be used
		IsDeleted = 0x10,
		IsNotCompressed = 0x10000,
	} ECpkEntityFlag;

#pragma pack(push,1)
	struct CpkEntity
	{
	public:
		uint32_t CRC;
		uint32_t Flag;
		uint32_t FatherCRC;
		uint32_t StartPos;
		uint32_t PackageSize;
		uint32_t OriginSize;
		uint32_t ExtraInfoSize;

		bool IsEmpty() const
		{
			return Flag == (uint32_t)ECpkEntityFlag::None;
		}

		bool IsCompressed() const
		{
			return (Flag & ((uint32_t)ECpkEntityFlag::IsNotCompressed)) == 0;
		}
		
		bool IsValid() const
		{
			return (Flag & ((uint32_t)ECpkEntityFlag::IsValid)) != 0;
		}

		bool IsDirectory() const
		{
			return (Flag & ((uint32_t)ECpkEntityFlag::IsDir)) != 0;
		}

		bool IsDeleted() const
		{
			return (Flag & ((uint32_t)ECpkEntityFlag::IsDeleted)) != 0;
		}
	};
#pragma pack(pop)
}
