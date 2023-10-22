#pragma once
#include <cstdint>
#include "../GameBoxType.h"
#include <vector>

namespace pal3
{
	class RawTexture;
	class TGALoader
	{
	public:
		RawTexture* Load(uint8_t* rawData,uint32_t len);
	};
}
