#include "RawAssetCache.h"
#include "Core/Public/Logging/LogMacros.h"
#include "Core/Public/Misc/Paths.h"

namespace pal3
{
	RawAssetCache::RawAssetCache()
	{
		_settings = pal3::Game::Instance()->GetService<pal3::Settings>();
		_cpkFileSys = pal3::Game::Instance()->GetService<pal3::CpkFileSystem>();
	}

	RawAssetCache::~RawAssetCache()
	{
		
	}

	std::string RawAssetCache::GenerateResKey(const std::string& cpkFileName, const std::string& relativePath)
	{
		std::string key = cpkFileName + "#" + relativePath;
		return key;
	}

	Mv3* RawAssetCache::GetMv3(const std::string& cpkFileName, const std::string& relativePath)
	{
		std::string key = GenerateResKey(cpkFileName, relativePath);
		auto it = _mv3Cache.find(key);
		if (it == _mv3Cache.end())
		{
			Mv3* mv3 = ReadMv3(cpkFileName, relativePath);
			_mv3Cache.insert(std::make_pair(key,mv3));
			return mv3;
		}
		return it->second;
	}

	RawTexture* RawAssetCache::GetTexture(const std::string& cpkFileName, const std::string& relativePath)
	{
		std::string key = GenerateResKey(cpkFileName, relativePath);
		auto it = _rawTextureCache.find(key);
		if (it == _rawTextureCache.end())
		{
			RawTexture* rawTexture = LoadRawTexture(cpkFileName,relativePath);
			_rawTextureCache.insert(std::make_pair(key,rawTexture));
			return rawTexture;
		}
		return it->second;
	}

	pal3::Mv3* RawAssetCache::ReadMv3(const std::string& cpkFileName, const std::string& relativePath)
	{
		std::string cpkPath = _settings->GetResourceRoot() + cpkFileName;

		_cpkFileSys = pal3::Game::Instance()->GetService<pal3::CpkFileSystem>();
		auto archive = _cpkFileSys->GetArchive(cpkPath);

		uint32_t len;
		uint8_t* data = archive->GetFileBytesFromFile(relativePath, len);

		pal3::Mv3Reader reader(data, len);
		pal3::Mv3* mv3 = reader.Read();

		return mv3;
	}

	RawTexture* RawAssetCache::LoadRawTexture(const std::string& cpkFileName, const std::string& relativePath)
	{
		std::string cpkPath = _settings->GetResourceRoot() + cpkFileName;

		auto cpkFileSys = pal3::Game::Instance()->GetService<pal3::CpkFileSystem>();
		auto archive = cpkFileSys->GetArchive(cpkPath);

		uint32_t len;
		uint8_t* data = archive->GetFileBytesFromFile(relativePath, len);

		pal3::TGALoader loader;
		pal3::RawTexture* rawTexture = loader.Load(data, len);

		return rawTexture;
	}
}
