#pragma once

#include <string>
#include <map>
#include <cstdint>

namespace pal3
{
	class CpkEntity;
	class Crc32Hash;
	struct CpkHeader;
	class CpkArchive
	{
	public:
		CpkArchive(const std::string& filePath,Crc32Hash* crcHash,int codePage);
		~CpkArchive();

		void Init();

	private:
		bool IsValidCpkHeader(const CpkHeader* header);


	private:
		std::string _filePath;
		Crc32Hash*	_crcHash;
		int _codePage;

		std::map<uint32_t, CpkEntity*>	_entities;
		std::map<uint32_t, std::string> _fileNameMap;
		std::map<uint32_t, uint32_t> _crcToTableIndexMap;
	};
}
