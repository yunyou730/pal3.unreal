#pragma once
#include <cstdint>
#include "Scn.h"

namespace pal3
{
	//struct Scn;
	class ScnReader
	{
	public:
		ScnReader(uint8_t* data,uint32_t len);
		Scn* Read();

	private:

	private:
		uint8_t* _data = nullptr;
		uint32_t _len = 0;
		uint32_t _cur = 0;
	};
}
