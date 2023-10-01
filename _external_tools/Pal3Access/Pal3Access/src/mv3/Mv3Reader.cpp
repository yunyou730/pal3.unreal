#include "../../headers//mv3/Mv3Reader.h"
#include "../../headers//mv3/Mv3.h"
#include <cassert>

namespace pal3
{
	Mv3Reader::Mv3Reader(uint8_t* data, uint32_t len)
		:_data(data)
		, _len(len)
		, _cur(0)
	{

	}

	Mv3* Mv3Reader::Read()
	{
		Mv3* result = new Mv3();

		// header
		char header[4];
		memcpy(header, _data + _cur, sizeof(header));
		assert(strcmp(header, "MV3") == 0);
		_cur += sizeof(header);

		// version
		uint32_t version = 0;
		memcpy(&version, _data + _cur, sizeof(uint32_t));
		assert(version == 100);
		_cur += sizeof(uint32_t);

		uint32_t duration;
		memcpy(&duration, _data + _cur, sizeof(uint32_t));
		_cur += sizeof(uint32_t);

		int32_t numOfMaterials;
		memcpy(&numOfMaterials, _data + _cur, sizeof(int32_t));
		_cur += sizeof(int32_t);

		int32_t numOfTagNodes;
		memcpy(&numOfTagNodes, _data + _cur, sizeof(int32_t));
		_cur += sizeof(int32_t);

		int32_t numOfMeshes;
		memcpy(&numOfMeshes, _data + _cur, sizeof(int32_t));
		_cur += sizeof(int32_t);

		int32_t numOfAnimationEvents;
		memcpy(&numOfAnimationEvents, _data + _cur, sizeof(int32_t));
		_cur += sizeof(int32_t);

		assert(numOfMeshes > 0 && numOfMaterials > 0);
		
		// animation events
		result->animationEvents.reserve(numOfAnimationEvents);
		for (int i = 0;i < numOfAnimationEvents;i++)
		{
			result->animationEvents.push_back(ReadAnimationEvent());
		}

		// tag nodes
		result->tagNodes.reserve(numOfTagNodes);
		for (int i = 0;i < numOfTagNodes;i++)
		{
			result->tagNodes.push_back(ReadTagNode());
		}

		// materials
		result->materials.reserve(numOfMaterials);
		for (int i = 0;i < numOfMaterials;i++)
		{
			result->materials.push_back(ReadMaterial());
		}

		// meshes
		result->meshes.reserve(numOfMeshes);
		for (int i = 0;i < numOfMeshes;i++)
		{
			result->meshes.push_back(ReadMesh());
		}
		
		return result;
	}
	
	Mv3AnimationEvent* Mv3Reader::ReadAnimationEvent()
	{
		auto item = new Mv3AnimationEvent();
		memcpy(&item->gameBoxTick,_data + _cur,sizeof(uint32_t));
		_cur += sizeof(uint32_t);

		memcpy(&item->name,_data + _cur,sizeof(item->name));
		_cur += sizeof(item->name);

		return item;
	}

	Mv3TagNode* Mv3Reader::ReadTagNode()
	{
		auto item = new Mv3TagNode();
		memcpy(&item->name,_data + _cur,sizeof(item->name));
		_cur += sizeof(item->name);

		memcpy(&item->flipScale,_data + _cur,sizeof(uint32_t));	// 32bits for float
		_cur += sizeof(uint32_t);


		int32_t numOfFrames;
		memcpy(&numOfFrames,_data + _cur,sizeof(int32_t));
		_cur += sizeof(int32_t);

		item->tagFrames.reserve(numOfFrames);
		for (int i = 0;i < numOfFrames;i++)
		{
			item->tagFrames.push_back(ReadTagFrame());
		}

		return item;
	}

	Mv3TagFrame* Mv3Reader::ReadTagFrame()
	{
		Mv3TagFrame* item = new Mv3TagFrame();

		memcpy(&item->gameBoxTick, _data + _cur, sizeof(uint32_t));
		_cur += sizeof(uint32_t);

		float xyz[3];
		memcpy(&xyz, _data + _cur, 3 * sizeof(uint32_t));	// 32 bits for float
		_cur += 3 * sizeof(uint32_t);
		item->gameBoxPosition.x = xyz[0];
		item->gameBoxPosition.y = xyz[1];
		item->gameBoxPosition.z = xyz[2];
		
		float xyzw[4];
		memcpy(&xyzw,_data + _cur, 4 * sizeof(uint32_t));	// 32 bits for float
		_cur += 4 * sizeof(uint32_t);
		item->gameBoxRotation.x = xyzw[0];
		item->gameBoxRotation.y = xyzw[1];
		item->gameBoxRotation.z = xyzw[2];
		item->gameBoxRotation.w = xyzw[3];

		memcpy(item->scale,_data + _cur, 9 * sizeof(uint32_t));	// 32 bits for float
		_cur += 9 * sizeof(uint32_t);

		return item;

	}

	GameBoxMaterial* Mv3Reader::ReadMaterial()
	{
		auto item = new GameBoxMaterial();

		float rgba[4];
		
		memcpy(rgba,_data + _cur,4 * sizeof(uint32_t));
		_cur += 4 * sizeof(uint32_t);
		item->diffuse.R = rgba[0];
		item->diffuse.G = rgba[1];
		item->diffuse.B = rgba[2];
		item->diffuse.A = rgba[3];

		memcpy(rgba, _data + _cur, 4 * sizeof(uint32_t));
		_cur += 4 * sizeof(uint32_t);
		item->ambient.R = rgba[0];
		item->ambient.G = rgba[1];
		item->ambient.B = rgba[2];
		item->ambient.A = rgba[3];

		memcpy(rgba, _data + _cur, 4 * sizeof(uint32_t));
		_cur += 4 * sizeof(uint32_t);
		item->specular.R = rgba[0];
		item->specular.G = rgba[1];
		item->specular.B = rgba[2];
		item->specular.A = rgba[3];

		memcpy(rgba, _data + _cur, 4 * sizeof(uint32_t));
		_cur += 4 * sizeof(uint32_t);
		item->emissive.R = rgba[0];
		item->emissive.G = rgba[1];
		item->emissive.B = rgba[2];
		item->emissive.A = rgba[3];

		memcpy(&item->specularPower, _data + _cur, sizeof(uint32_t));
		_cur += sizeof(uint32_t);


		// textures
		item->textureFileNames.reserve(4);
		for (int i = 0;i < 4;i++)
		{
			//std::string textureName;
			char* textureName = nullptr;

			int32_t len;
			memcpy(&len,_data + _cur,sizeof(int32_t));
			_cur += sizeof(int32_t);

			if (len > 0)
			{
				textureName = new char[len + 1];	// @leak!
				memcpy(textureName, _data + _cur, len);
				_cur += len;
				textureName[len] = '\0';
			}
			else
			{
				if (i == 0)
					continue;
				textureName = new char[1];	// @leak!
				textureName[0] = '\0'; // len = 0, textureName = "", use default white texture
			}
			
			//item->textureFileNames[i] = std::string(textureName);

			std::string textureFileName(textureName);
			item->textureFileNames.push_back(textureFileName);
		}

		return item;
	}

	Mv3Mesh* Mv3Reader::ReadMesh()
	{
		Mv3Mesh* item = new Mv3Mesh();
		// Name
		memcpy(item->name, _data + _cur, 64);
		_cur += 64;

		// Vertices Number
		int32_t numOfVertices;
		memcpy(&numOfVertices, _data + _cur, sizeof(int32_t));
		_cur += sizeof(int32_t);

		// Bounding Box		
		float xyz[3];
		memcpy(xyz, _data + _cur, 3 * sizeof(int32_t));
		_cur += 3 * sizeof(int32_t);
		item->gameboxBoundsMin.x = xyz[0];
		item->gameboxBoundsMin.y = xyz[1];
		item->gameboxBoundsMin.z = xyz[2];

		memcpy(xyz, _data + _cur, 3 * sizeof(int32_t));
		_cur += 3 * sizeof(int32_t);
		item->gameboxBoundsMax.x = xyz[0];
		item->gameboxBoundsMax.y = xyz[1];
		item->gameboxBoundsMax.z = xyz[2];

		// Frames
		int32_t numOfFrames;
		memcpy(&numOfFrames,_data + _cur,sizeof(int32_t));
		_cur += sizeof(int32_t);

		std::vector<Mv3VertFrame*> frames;
		frames.reserve(numOfFrames);
		for (int i = 0;i < numOfFrames;i++)
		{
			// Tick Index
			uint32_t gameBoxTick;
			memcpy(&gameBoxTick,_data + _cur,sizeof(uint32_t));
			_cur += sizeof(uint32_t);

			// Vertices
			std::vector<Mv3Vert*> vertices;
			vertices.reserve(numOfVertices);
			for (int j = 0;j < numOfVertices;j++)
			{
				Mv3Vert* vert = new Mv3Vert();
				memcpy(vert, _data + _cur, sizeof(Mv3Vert));
				_cur += sizeof(Mv3Vert);

				vertices.push_back(vert);
			}

			// Frame
			auto frame = new Mv3VertFrame();
			frame->gameBoxTick = gameBoxTick;
			frame->vertices = vertices;
			frames.push_back(frame);
		}

		// UVs
		int32_t numOfTexCoords;
		memcpy(&numOfTexCoords, _data + _cur, sizeof(int32_t));
		_cur += sizeof(int32_t);

		std::vector<GameBoxVector2> texCoords;
		if (numOfTexCoords == 0)
		{
			texCoords.push_back(GameBoxVector2(0,0));
		}
		else
		{
			for (int i = 0;i < numOfTexCoords;i++)
			{
				float uv[2];
				memcpy(uv, _data + _cur, 2 * sizeof(int32_t));
				_cur += 2 * sizeof(int32_t);

				texCoords.push_back(GameBoxVector2(uv[0], uv[1]));
			}
		}

		// Attributes
		int32_t numOfAttributes;
		memcpy(&numOfAttributes, _data + _cur, sizeof(int32_t));
		_cur += sizeof(int32_t);
		assert(numOfAttributes > 0);
		
		std::vector<Mv3Attribute*> attributes;
		attributes.reserve(numOfAttributes);
		for (int i = 0;i < numOfAttributes;i++)
		{
			Mv3Attribute* attribute = new Mv3Attribute();
			attributes.push_back(attribute);

			int32_t materialId, numOfTriangles;

			memcpy(&materialId, _data + _cur, sizeof(int32_t));
			_cur += sizeof(int32_t);

			memcpy(&numOfTriangles, _data + _cur, sizeof(int32_t));
			_cur += sizeof(int32_t);

			// Triangle & Index Buffer
			std::vector<Mv3IndexBuffer*> triangles;
			triangles.reserve(numOfTriangles);

			for (int j = 0;j < numOfTriangles;j++)
			{
				Mv3IndexBuffer* triangle = new Mv3IndexBuffer();

				memcpy(triangle, _data + _cur, sizeof(Mv3IndexBuffer));
				_cur += sizeof(Mv3IndexBuffer);

				triangles.push_back(triangle);
			}

			// Commands
			int32_t numOfCommands;
			memcpy(&numOfCommands, _data + _cur, sizeof(int32_t));
			_cur += sizeof(int32_t);

			if (numOfCommands > 0)
			{
				attribute->commands.reserve(numOfCommands);
				memcpy(attribute->commands.data(), _data + _cur, numOfCommands * sizeof(int32_t));
				_cur += numOfCommands * sizeof(int32_t);
			}
			
			// Attribute
			attribute->materialId = materialId;
			attribute->indexBuffers = triangles;
		}
		
		BuildMesh(item, attributes, frames, texCoords);
		return item;
	}

	void Mv3Reader::BuildMesh(Mv3Mesh* targetMesh,
								const std::vector<Mv3Attribute*>& attributes,
								const std::vector<Mv3VertFrame*>& vertFrames,
								const std::vector<GameBoxVector2>& texCoords)
	{
		std::vector<uint32_t> triangles(attributes[0]->indexBuffers.size() * 3);
		std::vector<GameBoxVector2> uvs(attributes[0]->indexBuffers.size() * 3);

		// idx1: key index; idx2: Vert Position
		std::vector<std::vector<GameBoxVector3>> keyFrameVertices(vertFrames.size());
		
		uint32_t triangleIndex = 0;

		// Each face
		for (int i = 0;i < attributes[0]->indexBuffers.size();i++)
		{
			Mv3IndexBuffer* indexBuffer = attributes[0]->indexBuffers[i];

			// Each vert
			for (int j = 0;j < 3;j++)
			{
				// Each frame
				for (int k = 0;k < vertFrames.size();k++)
				{
					Mv3VertFrame* frame = vertFrames[k];
					Mv3Vert* vert = frame->vertices[indexBuffer->triangleIndex[j]];
					keyFrameVertices[k].push_back(GameBoxVector3(vert->x,vert->y,vert->z));
				}

				uvs[triangleIndex] = texCoords[indexBuffer->texCoordIndex[j]];
				triangles[triangleIndex] = triangleIndex;
				triangleIndex++;
			}
		}

		// Key frames
		std::vector<Mv3AnimationKeyFrame*> animationKeyFrames(vertFrames.size());
		for (int i = 0;i < animationKeyFrames.size();i++)
		{
			auto item = new Mv3AnimationKeyFrame();
			animationKeyFrames[i] = item;

			item->gameBoxTick = vertFrames[i]->gameBoxTick;
			item->gameBoxVertices = keyFrameVertices[i];
		}

		// set to target mesh
		targetMesh->keyFrames = animationKeyFrames;
		targetMesh->UVs = uvs;
		targetMesh->gameboxTriangles = triangles;
		// @todo
		//targetMesh->gameboxNormals;
	}
}

