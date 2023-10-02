#pragma once
#include <cstdint>
#include "../GameBoxType.h"
#include <vector>

namespace pal3
{
	struct Mv3AnimationEvent;
	struct Mv3TagFrame;
	struct Mv3TagNode;
	struct Mv3IndexBuffer;
	struct Mv3Attribute;
	struct Mv3Mesh;

	struct Mv3
	{
		uint32_t totalGameBoxTicks;
		std::vector<Mv3AnimationEvent*> animationEvents;
		std::vector<Mv3TagNode*> tagNodes;
		std::vector<Mv3Mesh*> meshes;
		std::vector<GameBoxMaterial*> materials;
	};

	struct Mv3AnimationEvent
	{
		uint32_t gameBoxTick;
		char name[16];
	};
	

	struct Mv3TagFrame
	{
		uint32_t gameBoxTick;
		GameBoxVector3 gameBoxPosition;
		GameBoxQuaternion gameBoxRotation;
		float scale[3][3];
	};

	struct Mv3TagNode
	{
		char name[64];
		float flipScale;
		std::vector<Mv3TagFrame*> tagFrames;
	};
#pragma pack(push,1)
	struct Mv3IndexBuffer
	{
		uint16_t triangleIndex[3];
		uint16_t texCoordIndex[3];
	};
#pragma pack(pop)

	struct Mv3Attribute
	{
		uint32_t materialId;
		std::vector<Mv3IndexBuffer*> indexBuffers;
		std::vector<uint32_t> commands;
	};

#pragma pack(push,1)
	struct Mv3Vert
	{
		int16_t X;
		int16_t Y;
		int16_t Z;
		uint16_t N;
	};
#pragma pack(pop)

	struct Mv3VertFrame
	{
		uint32_t gameBoxTick;
		std::vector<Mv3Vert*> vertices;
	};

	struct Mv3AnimationKeyFrame
	{
		uint32_t gameBoxTick;
		std::vector<GameBoxVector3> gameBoxVertices;
	};

	struct Mv3Mesh
	{
		char name[64];
		GameBoxVector3 gameboxBoundsMin;
		GameBoxVector3 gameboxBoundsMax;

		std::vector<Mv3Attribute*> attributes;
		std::vector<GameBoxVector3> gameboxNormals;
		std::vector<uint32_t> gameboxTriangles;
		std::vector<GameBoxVector2> UVs;
		std::vector<Mv3AnimationKeyFrame*> keyFrames;
	};
}
