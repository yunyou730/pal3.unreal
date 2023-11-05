#include "Mv3Wrapper.h"

#include "Pal3Ext/GameBoxConverter.h"

namespace pal3
{
	Mv3Wrapper::Mv3Wrapper()
	{
		
	}

	Mv3Wrapper::~Mv3Wrapper()
	{
		if (_locator != nullptr)
		{
			delete _locator;
			_locator = nullptr;
		}
	}

	void Mv3Wrapper::Init(const std::string& cpkFile, const std::string& relativeDir, const std::string& fileName)
	{
		_locator = new pal3::FileLocator(cpkFile,relativeDir,fileName);
		_rawAssetCache = pal3::Game::Instance()->GetService<pal3::RawAssetCache>();

		LoadMv3();
		LoadRawTextures();
	}

	void Mv3Wrapper::LoadMv3()
	{
		_mv3 = _rawAssetCache->GetMv3(_locator->_cpkName, _locator->_relativeDirectory + "\\" + _locator->_fileName);
	}

	void Mv3Wrapper::LoadRawTextures()
	{
		for (size_t materialIdx = 0; materialIdx < _mv3->materials.size(); materialIdx++)
		{
			pal3::GameBoxMaterial* mat = _mv3->materials[materialIdx];

			std::vector<RawTexture*> textures;
			for (size_t textureIdx = 0;textureIdx < mat->textureFileNames.size();textureIdx++)
			{
				std::string textureFileName = mat->textureFileNames[textureIdx];
				if (!textureFileName.empty())
				{
					std::string textureFilePath = _locator->GetFilePathInSameDir(textureFileName);

					RawTexture* rawTexture = _rawAssetCache->GetTexture(_locator->_cpkName, textureFilePath);
					textures.push_back(rawTexture);
				}
			}

			_rawTextures.push_back(textures);
		}
	}

	uint32_t Mv3Wrapper::GetSubMeshCount()
	{
		return _mv3->meshes.size();
	}

	uint32_t Mv3Wrapper::GetFrameCount()
	{
		return _mv3->meshes[0]->keyFrames.size();
	}

	void Mv3Wrapper::GetVerticesAtFrameIndex(
		int subMeshIndex,
		uint32_t frameIndex,
		TArray<FVector>& vertices,
		TArray<int32_t>& triangles,
		TArray<FVector>& normals,
		TArray<FVector2D>& uv0)
	{

		int32_t vertCount = _mv3->meshes[subMeshIndex]->keyFrames[frameIndex]->gameBoxVertices.size();		
		for (auto it : _mv3->meshes[subMeshIndex]->keyFrames[frameIndex]->gameBoxVertices)
		{
			vertices.Add(FVector(it.x, it.y, it.z));
			normals.Add(FVector(0, 0, 1));		// @miao @temp; // normal we should re-calculate it 
		}

		
		//for (auto it : mv3->meshes[subMeshIndex]->gameboxTriangles)
		//{
		//	triangles.Add(it);
		//}

		// triangle winding very important
		uint32_t triangleCount = _mv3->meshes[subMeshIndex]->gameboxTriangles.size() / 3;
		for (uint32_t triangleIdx = 0; triangleIdx < triangleCount; triangleIdx++)
		{
			uint32_t idx1 = triangleIdx * 3;
			uint32_t idx2 = idx1 + 2;
			uint32_t idx3 = idx1 + 1;

			triangles.Add(_mv3->meshes[subMeshIndex]->gameboxTriangles[idx1]);
			triangles.Add(_mv3->meshes[subMeshIndex]->gameboxTriangles[idx2]);
			triangles.Add(_mv3->meshes[subMeshIndex]->gameboxTriangles[idx3]);
		}

		
		for (auto it : _mv3->meshes[subMeshIndex]->UVs)
		{
			uv0.Add(FVector2D(it.x, it.y));
		}

		pal3ext::GameBoxConverter::ConvertVertices(vertices);
	}

	// temp only return the 1st texture
	RawTexture* Mv3Wrapper::GetTextureAtMeshIndex(size_t subMeshIndex)
	{
		RawTexture* rawTexture = _rawTextures[subMeshIndex][0];
		return rawTexture;
	}
}