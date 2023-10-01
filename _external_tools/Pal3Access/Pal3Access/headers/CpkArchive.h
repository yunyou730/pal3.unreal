#pragma once

#include <string>
#include <map>
#include <cstdint>
#include <set>
#include <vector>

namespace pal3
{
	struct CpkEntity;
	class Crc32Hash;
	struct CpkHeader;
	class CpkEntry;
	class CpkArchive
	{
	public:
		CpkArchive(const std::string& filePath,Crc32Hash* crcHash,int codePage);
		~CpkArchive();

		void Init();
		bool FileExists(const std::string& fileVirtualPath);
		void LoadArchiveIntoMemory();
		void ReleaseMemory();
		void ExtractTo(const std::string& outputFolder);
		uint8_t* GetFileBytes(const std::string& fileVirtualPath, uint32_t& len, bool& bNeedDealloc);

		void SetLogEnable(bool bEnable) { _bEnableLog = bEnable; }

	private:
		bool IsValidCpkHeader(const CpkHeader* header);
		void BuildEntities(const uint8_t* indexTableBuffer, const CpkHeader& header);
		void BuildCrcIndexMap();
		CpkEntity* GetCpkEntity(const std::string& fileVirtualPath);
		
		void ExtractToInternal(const std::string& outputFolder,const std::vector<CpkEntry*>& nodes);

		std::vector<CpkEntry*> BuildEntryTree();
		void BuildFileNameMap();
		std::vector<CpkEntry*> GetChildren(uint32_t fatherCRC, std::string rootPath);

	private:
		std::string _filePath;
		Crc32Hash*	_crcHash;
		int _codePage;

		// Key: file index; Value: CpkEntity* 
		std::map<uint32_t, CpkEntity*>	_entities;
		
		// Key: file name CRC; Value: file name string
		std::map<uint32_t, std::string> _fileNameMap;	

		// Key: file name CRC; Value: file index
		std::map<uint32_t, uint32_t> _crcToTableIndexMap;

		// Key: directory name CRC(also file name,but entity is directory); Value: files name CRC set
		std::map<uint32_t, std::set<uint32_t>> _fatherCrcToChildCrcTableIndexMap;

		// Hold all binary data
		uint8_t* _archiveData = nullptr;
		size_t _archiveDataLen = 0;

		bool _bEnableLog = false;
	};
}
