#include "CpkArchive.h"
#include "CrcHash.h"
#include <string>
#include "CpkHeader.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include "CpkEntity.h"

namespace pal3
{
	const int32_t CPK_VERSION = 1;
	const int32_t CPK_HEADER_MAGIC = 0x1A545352;  // CPK header magic label
	const int32_t CPK_DEFAULT_MAX_NUM_OF_FILE = 32768; // Max number of files per archive

	CpkArchive::CpkArchive(const std::string& filePath, Crc32Hash* crcHash, int codePage)
	{
		_filePath = filePath;
		_crcHash = crcHash;
		_codePage = codePage;
	}

	CpkArchive::~CpkArchive()
	{
		
	}

	void CpkArchive::Init()
	{
		std::ifstream file(_filePath,std::ios::binary);
		if (file)
		{
			CpkHeader header;
			file.read(reinterpret_cast<char*>(&header),sizeof(header));

			if (!IsValidCpkHeader(&header))
			{
				
			}

			size_t cpkEntitySize = sizeof(CpkEntity);



			file.close();
		}
	}

	bool CpkArchive::IsValidCpkHeader(const CpkHeader* header)
	{
		if (header->label != CPK_HEADER_MAGIC) return false;
		if (header->version != CPK_VERSION) return false;
		if (header->tableStart == 0) return false;
		if (header->fileNum > header->maxFileNum) return false;
		if (header->validTableNum > header->maxTableNum) return false;
		if (header->fileNum > header->validTableNum) return false;

		return true;
	}
}
