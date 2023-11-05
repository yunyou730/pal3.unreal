#pragma once
#include "../../headers/textureloader/TGALoader.h"
#include "../../headers/textureloader/RawTexture.h"
#include "../../headers/BinReader.h"
#include <cassert>

namespace pal3
{
	RawTexture* TGALoader::Load(uint8_t* rawData, uint32_t len)
	{
		RawTexture* ret = new RawTexture();

		uint8_t* p = rawData + 12;

		uint16_t width = 0;
		uint16_t height = 0;
		uint8_t bitDepth = 0;

		uint32_t ptr = 0;

		// base info
		BinReader::ReadShort(width, p, ptr);
		BinReader::ReadShort(height, p, ptr);
		bitDepth = BinReader::Read<uint8_t>(p,ptr);

		// pixels
		assert(bitDepth == 32);

		size_t pixels = width * height;
		switch (bitDepth)
		{
		case 32:
		{
			//uint32_t idx = 0;
			ret->bytes.reserve(4 * pixels);
			for (size_t i = 0; i < pixels; i++)
			{
				// RGBA for one pixel
				uint8_t a = BinReader::Read<uint8_t>(p, ptr);
				uint8_t b = BinReader::Read<uint8_t>(p, ptr);
				uint8_t g = BinReader::Read<uint8_t>(p, ptr);
				uint8_t r = BinReader::Read<uint8_t>(p, ptr);

				// r = 0;g = 0;b = 255;a = 255;
				ret->bytes.push_back(r);
				ret->bytes.push_back(g);
				ret->bytes.push_back(b);
				ret->bytes.push_back(a);
			}
		}
			break;
		case 24:
		{
			ret->bytes.reserve(4 * pixels);
			for (size_t i = 0; i < pixels; i++)
			{
				// RGB for one pixel
				uint8_t r = BinReader::Read<uint8_t>(p, ptr);
				uint8_t b = BinReader::Read<uint8_t>(p, ptr);
				uint8_t g = BinReader::Read<uint8_t>(p, ptr); 
				uint8_t a = 255;

				ret->bytes.push_back(r);
				ret->bytes.push_back(g);
				ret->bytes.push_back(b);
				ret->bytes.push_back(a);
			}
		}
			break;
		default:
			break;
		}

		ret->width = width;
		ret->height = height;
		return ret;
	}
}
