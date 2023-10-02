#pragma once
#include <string>
#include <vector>

namespace pal3
{
	class BinReader
	{
	public:
		static void Read(void* dest,uint8_t* src, uint32_t& ptr, size_t amount)
		{
			memcpy(dest, src + ptr, amount);
			ptr += amount;
		}

		template<typename T>
		static void Read(T& value, uint8_t* src, uint32_t& ptr)
		{
			memcpy(&value, src + ptr, sizeof(T));
			ptr += sizeof(T);
		}

		template<typename T>
		static T Read(uint8_t* src, uint32_t& ptr)
		{
			T value;
			memcpy(&value, src + ptr, sizeof(T));
			ptr += sizeof(T);
			return value;
		}

		static void ReadColor(GameBoxColor& color,uint8_t* src,uint32_t& ptr)
		{
			float rgba[4];
			Read(rgba,src,ptr, 4 * sizeof(uint32_t));
			color.R = rgba[0];
			color.G = rgba[1];
			color.B = rgba[2];
			color.A = rgba[3];
		}

		static void ReadFloat(float& value,uint8_t* src,uint32_t& ptr)
		{
			Read(&value,src,ptr,sizeof(uint32_t)); // float size equals 32
		}

		static void ReadVector3(GameBoxVector3& value,uint8_t* src,uint32_t& ptr)
		{
			float xyz[3];
			BinReader::Read(&xyz, src, ptr, 3 * sizeof(uint32_t));	// float size is 32
			value.x = xyz[0];
			value.y = xyz[1];
			value.z = xyz[2];
		}

		static void ReadQuaternion(GameBoxQuaternion& value,uint8_t* src,uint32_t& ptr)
		{
			float xyzw[4];
			BinReader::Read(&xyzw, src, ptr, 4 * sizeof(uint32_t));	// 32 bits for float
			value.x = xyzw[0];
			value.y = xyzw[1];
			value.z = xyzw[2];
			value.w = xyzw[3];
		}



	};
}
