#pragma once
#include <cstdint>
#include <vector>

namespace pal3
{
	class RawTexture
	{
	public:
		int width;
		int height;
		std::vector<uint8_t> bytes;	// RGBA,RGBA ...
	};
}
