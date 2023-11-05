#pragma once
#include <string>
#include <vector>

#include "Pal3Core/headers/CpkFileSystem.h"
#include "Pal3Core/headers/cpk/CpkArchive.h"
#include "Pal3Core/headers/mv3/Mv3Reader.h"
#include "Pal3Core/headers/mv3/Mv3.h"
#include "Pal3Core/headers/textureloader/RawTexture.h"
#include "Pal3Core/headers/textureloader/TGALoader.h"

#include "FileLocator.h"

#include "Pal3Game/Game.h"
#include "Pal3Game/Settings.h"
#include "Pal3Game/RawAssetCache.h"

#include "CoreMinimal.h"
//#include "ProceduralMeshComponent.h"

namespace pal3
{
	class Mv3Wrapper
	{
	public:
		Mv3Wrapper();
		~Mv3Wrapper();

		void Init(const std::string& cpkFile,const std::string& relativeDir,const std::string& fileName);
		RawTexture* GetTextureAtMeshIndex(size_t subMeshIndex);

		uint32_t GetSubMeshCount();
		uint32_t GetFrameCount();


		void GetVerticesAtFrameIndex(int subMeshIndex,
									uint32_t frameIndex,
									TArray<FVector>& vertices, 
									TArray<int32_t>& triangles,
									TArray<FVector>& normals,
									TArray<FVector2D>& uv0);

		bool HasLoaded() { return _mv3 != nullptr; }
	private:
		void LoadMv3();
		void LoadRawTextures();
		
	private:
		FileLocator* _locator;
		pal3::RawAssetCache* _rawAssetCache = nullptr;
		Mv3* _mv3 = nullptr;

		std::vector<std::vector<pal3::RawTexture*>>	_rawTextures;		// index 1: material index; index 2: texture index
	};
}
