#pragma once

#include "Pal3Game/Game.h"
#include "Pal3Game/Settings.h"

#include "Pal3Core/headers/mv3/Mv3.h"
#include "Pal3Core/headers/textureloader/RawTexture.h"

#include "Core/Public/CoreTypes.h"
#include <string>
#include <map>

#include "Pal3Core/headers/CpkFileSystem.h"
#include "Pal3Core/headers/cpk/CpkArchive.h"
#include "Pal3Core/headers/mv3/Mv3Reader.h"
#include "Pal3Core/headers/mv3/Mv3.h"
#include "Pal3Core/headers/textureloader/RawTexture.h"
#include "Pal3Core/headers/textureloader/TGALoader.h"

namespace pal3
{
	class RawAssetCache
	{
	public:
		RawAssetCache();
		~RawAssetCache();

	public:
		std::string GenerateResKey(const std::string& cpkFileName,const std::string& relativePath);

		Mv3* GetMv3(const std::string& cpkFileName, const std::string& relativePath);
		RawTexture* GetTexture(const std::string& cpkFileName, const std::string& relativePath);

	private:
		Mv3* ReadMv3(const std::string& cpkFileName, const std::string& relativePath);
		RawTexture* LoadRawTexture(const std::string& cpkFileName, const std::string& relativePath);

	private:
		std::map<std::string, pal3::Mv3*> _mv3Cache;
		std::map<std::string, pal3::RawTexture*> _rawTextureCache;

		pal3::Settings* _settings = nullptr;
		pal3::CpkFileSystem* _cpkFileSys = nullptr;
	};
}