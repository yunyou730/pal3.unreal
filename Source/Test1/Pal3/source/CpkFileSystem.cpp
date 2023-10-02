#include "../headers/CpkFileSystem.h"
#include "../headers/cpk/CrcHash.h"
#include "../headers/cpk/CpkArchive.h"

namespace pal3
{
	static const int GB2312_CODE_PAGE = 936;	// gb2312

	CpkFileSystem::CpkFileSystem()
	{
		_crcHash = new pal3::Crc32Hash();
		_crcHash->Init();
		_codePage = GB2312_CODE_PAGE;
	}

	CpkFileSystem::~CpkFileSystem()
	{
		ReleaseMountedArchives();
		ReleaseCRCHash();
	}

	CpkArchive* CpkFileSystem::Mount(const std::string& cpkFilePath)
	{
		CpkArchive* archive = new CpkArchive(cpkFilePath,_crcHash,_codePage);
		archive->Init();
		return archive;
	}

	void CpkFileSystem::ReleaseCRCHash()
	{
		delete _crcHash;
		_crcHash = nullptr;
	}

	void CpkFileSystem::ReleaseMountedArchives()
	{
		for (auto it : _cpkArchives)
		{
			CpkArchive* archive = it.second;
			delete archive;
		}
		_cpkArchives.clear();
	}

}
