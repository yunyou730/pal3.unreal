#include "../../headers/cpk/CpkArchive.h"
#include "../../headers/cpk/CrcHash.h"
#include "../../headers/cpk/CpkHeader.h"
#include "../../headers/cpk/CpkEntity.h"
#include "../../headers/cpk/CpkEntry.h"
#include "../../headers/cpk/CpkUtils.h"
//#include "../headers/encoding/Encoding.h"
#include "./minilzo.h"
#include <string>
#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <cstring>
#include <algorithm>
#include <cctype>
#include <filesystem>

namespace pal3
{
	namespace fs = std::filesystem;

	static const int32_t CPK_VERSION = 1;
	static const int32_t CPK_HEADER_MAGIC = 0x1A545352;  // CPK header magic label
	static const int32_t CPK_DEFAULT_MAX_NUM_OF_FILE = 32768; // Max number of files per archive
	
	CpkArchive::CpkArchive(const std::string& filePath, Crc32Hash* crcHash, int codePage)
	{
		_filePath = filePath;
		_crcHash = crcHash;
		_codePage = codePage;
	}

	CpkArchive::~CpkArchive()
	{
		ReleaseEntities();
		ReleaseEntries();
		ReleaseMemory();
	}

	void CpkArchive::Init()
	{
		std::ifstream file(_filePath,std::ios::binary);
		if (file)
		{
			CpkHeader header;
			file.read(reinterpret_cast<char*>(&header),sizeof(CpkHeader));

			if (!IsValidCpkHeader(&header))
			{
				printf("Invalid Cpk Header %s\n",_filePath.c_str());
			}

			size_t cpkEntitySize = sizeof(CpkEntity);
			size_t indexTableSize = cpkEntitySize * CPK_DEFAULT_MAX_NUM_OF_FILE;

			uint8_t* indexTableBuffer = new uint8_t[indexTableSize];
			{
				// Read the whole index table into memory before processing ,to avoid I/O overhead
				file.read(reinterpret_cast<char*>(indexTableBuffer), indexTableSize);
				file.close();
				BuildEntities(indexTableBuffer, header);
			}
			delete[] indexTableBuffer;
			
			BuildCrcIndexMap();
		}
	}

	bool CpkArchive::FileExists(const std::string& fileVirtualPath)
	{
		std::string lowerFilePath = fileVirtualPath;
		std::transform(fileVirtualPath.begin(), fileVirtualPath.end(), lowerFilePath.begin(), [](unsigned char c) {
			return std::tolower(c);
		});

		uint32_t crc = _crcHash->Compute(lowerFilePath, _codePage);
		return _crcToTableIndexMap.find(crc) != _crcToTableIndexMap.end();
	}

	CpkEntity* CpkArchive::GetCpkEntity(const std::string& fileVirtualPath)
	{
		std::string lowerFilePath = fileVirtualPath;
		std::transform(fileVirtualPath.begin(), fileVirtualPath.end(), lowerFilePath.begin(), [](unsigned char c) {
			return std::tolower(c);
			});

		uint32_t crc = _crcHash->Compute(lowerFilePath, _codePage);
		auto it = _crcToTableIndexMap.find(crc);
		assert(it != _crcToTableIndexMap.end());

		CpkEntity* entity = _entities[_crcToTableIndexMap[crc]];		
		assert(!entity->IsDirectory());

		return entity;
	}

	void CpkArchive::LoadArchiveIntoMemory()
	{
		std::ifstream file(_filePath, std::ios::binary);
		if (file.is_open())
		{
			file.seekg(0,file.end);
			size_t size = file.tellg();
			_archiveData = new uint8_t[size];
			_archiveDataLen = size;

			file.seekg(0,file.beg);
			file.read(reinterpret_cast<char*>(_archiveData), size);
			file.close();
		}
	}

	void CpkArchive::ReleaseMemory()
	{
		if (_archiveData != nullptr)
		{
			delete[] _archiveData;
			_archiveData = nullptr;
		}
	}

	void CpkArchive::ExtractTo(const std::string& outputFolder)
	{
		assert(_archiveData != nullptr);

		_rootEntries = BuildEntryTree();
		CpkUtils::CreateDirectory(outputFolder);
		ExtractToInternal(outputFolder, _rootEntries);
	}

	void CpkArchive::ExtractToInternal(const std::string& outputFolder,const std::vector<CpkEntry*>& nodes)
	{
		std::string path = outputFolder;
		if (!CpkUtils::endsWithPathSeparator(path))
		{
			path = path + CpkUtils::PATH_SEQPARATOR;
		}

		uint32_t rawLen = 0;
		for (auto it : nodes)
		{
			CpkEntry* node = it;
			const std::string& reltivePath = node->VirtualPath;
			std::string fullPath = path + reltivePath;

			if (node->IsDirectory)
			{
				CpkUtils::CreateDirectory(fullPath);
				ExtractToInternal(outputFolder, node->Children);
			}
			else
			{
				bool bNeedDealloc = false;
				uint8_t* rawData = GetFileBytesFromCache(reltivePath, rawLen, bNeedDealloc);

				if (rawData != nullptr)
				{
					std::ofstream file(fullPath,std::ios::binary | std::ios::trunc);
					if (file.is_open())
					{
						file.write(reinterpret_cast<const char*>(rawData),rawLen);
						//printf("[WriteFile] %s\n", fullPath.c_str());
					}

					if (bNeedDealloc)
					{
						delete[] rawData;
					}
				}
			}
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

	void CpkArchive::BuildEntities(const uint8_t* indexTableBuffer,const CpkHeader& header)
	{
		size_t indexTableOffset = 0;
		size_t entitySize = sizeof(CpkEntity);

		size_t filesFound = 0;
		for (uint32_t i = 0; i < header.maxFileNum; i++)
		{
			CpkEntity* entity = new CpkEntity();

			// Fill entity with binary data
			memcpy(entity, indexTableBuffer + indexTableOffset, entitySize);
			indexTableOffset += entitySize;

			if (!entity->IsEmpty() && !entity->IsDeleted())
			{
				_entities.insert(std::make_pair(i, entity));

				filesFound++;
				if (header.fileNum == filesFound)
				{
					break;
				}
			}
			else
			{
				printf("There's a empty or deleted entity\n");
				delete entity;
			}
		}
	}

	void CpkArchive::BuildCrcIndexMap()
	{
		for (auto it = _entities.begin(); it != _entities.end(); it++)
		{
			uint32_t idx = it->first;
			CpkEntity* entity = it->second;

			// CRC to Index map
			_crcToTableIndexMap[entity->CRC] = idx;

			// Father CRC map
			auto it2 = _fatherCrcToChildCrcTableIndexMap.find(entity->FatherCRC);
			if (it2 != _fatherCrcToChildCrcTableIndexMap.end())
			{
				std::set<uint32_t>& entityCrcSet = it2->second;
				entityCrcSet.insert(entity->CRC);
			}
			else
			{
				std::set<uint32_t> entityCrcSet;
				entityCrcSet.insert(entity->CRC);
				_fatherCrcToChildCrcTableIndexMap.insert(std::make_pair(entity->FatherCRC, entityCrcSet));
			}

			/*if (entity->FatherCRC == 0)
			{
				printf("ROOT\n");
			}*/
		}
	}

	// Build a tree structure map of the internal file system,
	// and return the root nodes in CpkEntry format
	std::vector<CpkEntry*> CpkArchive::BuildEntryTree()
	{
		if (_fileNameMap.size() == 0)
		{
			BuildFileNameMap();
		}
		return GetChildren(0,"");
	}

	void CpkArchive::BuildFileNameMap()
	{
		for (auto it : _entities)
		{
			CpkEntity* entity = it.second;
			uint64_t extraInfoOffset = entity->StartPos + entity->PackageSize;
			//uint32_t extraInfoOffset = entity->StartPos + entity->PackageSize;

			uint8_t* extraInfo = new uint8_t[entity->ExtraInfoSize];
			memcpy(extraInfo, _archiveData + extraInfoOffset, entity->ExtraInfoSize);

			_fileNameMap[entity->CRC] = std::string(reinterpret_cast<char*>(extraInfo));

			delete[] extraInfo;

		}
	}

	std::vector<CpkEntry*> CpkArchive::GetChildren(uint32_t fatherCRC, std::string rootPath)
	{
		std::vector<CpkEntry*> result;

		// Check fatherCRC validate
		auto fatherIt = _fatherCrcToChildCrcTableIndexMap.find(fatherCRC);
		if (fatherIt == _fatherCrcToChildCrcTableIndexMap.end())
		{
			return result;
		}
		
		auto& childCrcSet = fatherIt->second;
		for (uint32_t childCrc : childCrcSet)
		{
			// file index
			auto indexIt = _crcToTableIndexMap.find(childCrc);
			assert(indexIt != _crcToTableIndexMap.end());
			uint32_t index = indexIt->second;

			// child CpkEntity
			auto childEntityIt = _entities.find(index);
			assert(childEntityIt != _entities.end());
			CpkEntity* childEntity = childEntityIt->second;

			// virtual file path
			std::string filePath = _fileNameMap[childEntity->CRC];
			//std::string encodingPath = Encoding::ConvertStringToGB2312(filePath);
			std::string virtualPath = rootPath + filePath;


			CpkEntry* entry = nullptr;
			if (childEntity->IsDirectory())
			{
				entry = new CpkEntry(virtualPath, true, GetChildren(childEntity->CRC, virtualPath + CpkUtils::PATH_SEQPARATOR));
			}
			else
			{
				entry = new CpkEntry(virtualPath, false);
				//printf("### %s\n",virtualPath.c_str());
			}
			result.push_back(entry);
		}
		return result;
	}

	uint8_t* CpkArchive::GetFileBytesFromCache(const std::string& fileVirtualPath, uint32_t& len,bool& bNeedDealloc)
	{
		CpkEntity* entity = GetCpkEntity(fileVirtualPath);
		uint32_t start = entity->StartPos;
		uint32_t end = entity->StartPos + entity->PackageSize;

		if (entity->IsCompressed())
		{
			uint32_t size = entity->OriginSize;
			len = 0;
			
			uint32_t rawLen = end - start;
			uint8_t* decompressedData = new uint8_t[size];	// Leak here! we should delete[] from outside

			lzo_uint decompressedLen;
			lzo1x_decompress(_archiveData + start, rawLen, decompressedData, &decompressedLen, nullptr);

			len = size;

			bNeedDealloc = true;
			return decompressedData;
		}
		else
		{
			len = end - start;
			bNeedDealloc = false;
			return _archiveData + start;
		}
	}

	uint8_t* CpkArchive::GetFileBytesFromFile(const std::string& fileVirtualPath, uint32_t& len)
	{
		CpkEntity* entity = GetCpkEntity(fileVirtualPath);
		uint32_t start = entity->StartPos;
		uint32_t end = entity->StartPos + entity->PackageSize;

		std::ifstream file(_filePath, std::ios::binary);
		assert(file.is_open());
		
		const uint32_t rawLen = end - start;
		uint8_t* rawData = new uint8_t[rawLen];
		
		file.seekg(start);
		file.read(reinterpret_cast<char*>(rawData), end - start);
		file.close();

		if (entity->IsCompressed())
		{
			len = entity->OriginSize;

			uint8_t* decompressedData = new uint8_t[len];
			lzo_uint decompressedLen;
			lzo1x_decompress(rawData, rawLen, decompressedData, &decompressedLen, nullptr);
			delete[] rawData;

			return decompressedData;
		}
		else
		{
			len = rawLen;
			return rawData;
		}
	}

	void CpkArchive::ReleaseEntities()
	{
		for (auto it : _entities)
		{
			CpkEntity* entity = it.second;
			delete entity;
		}
		_entities.clear();
	}

	void CpkArchive::ReleaseEntries()
	{
		for (auto it : _rootEntries)
		{
			auto entry = it;
			ReleaseEntriesInternal(entry);
		}
	}

	void CpkArchive::ReleaseEntriesInternal(CpkEntry* entry)
	{
		if (entry->IsDirectory)
		{
			for (auto it : entry->Children)
			{
				CpkEntry* childEntry = it;
				ReleaseEntriesInternal(childEntry);
			}
			entry->Children.clear();
			delete entry;
		}
		else
		{
			delete entry;
		}
	}
}
