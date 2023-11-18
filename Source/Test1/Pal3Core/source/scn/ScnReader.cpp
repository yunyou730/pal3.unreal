#include "../../headers/scn/ScnReader.h"
#include "../../headers/scn/Scn.h"
#include <cassert>
#include "../../headers/BinReader.h"

namespace pal3
{
	//Mv3Reader::Mv3Reader(uint8_t* data, uint32_t len)
	//	:_data(data)
	//	, _len(len)
	//	, _cur(0)
	//{

	//}

	//Mv3* Mv3Reader::Read()
	//{
	//	Mv3* result = new Mv3();

	//	char header[5];
	//	BinReader::Read(header, _data, _cur, 4 * sizeof(uint8_t));
	//	header[4] = '\0';
	//	assert(strcmp(header, "MV3") == 0);
	//	
	//	uint32_t version = 0;
	//	BinReader::Read(&version, _data, _cur, sizeof(uint32_t));
	//	assert(version == 100);
	//	
	//	uint32_t duration;
	//	BinReader::Read(&duration, _data, _cur, sizeof(uint32_t));

	//	int32_t numOfMaterials;
	//	BinReader::Read(&numOfMaterials, _data, _cur, sizeof(int32_t));

	//	int32_t numOfTagNodes;
	//	BinReader::Read(&numOfTagNodes, _data, _cur, sizeof(int32_t));

	//	int32_t numOfMeshes;
	//	BinReader::Read(&numOfMeshes, _data, _cur, sizeof(int32_t));

	//	int32_t numOfAnimationEvents;
	//	BinReader::Read(&numOfAnimationEvents, _data, _cur, sizeof(int32_t));
	//	
	//	assert(numOfMeshes > 0 && numOfMaterials > 0);
	//	
	//	// animation events
	//	result->animationEvents.reserve(numOfAnimationEvents);
	//	for (int i = 0;i < numOfAnimationEvents;i++)
	//	{
	//		result->animationEvents.push_back(ReadAnimationEvent());
	//	}

	//	// tag nodes
	//	result->tagNodes.reserve(numOfTagNodes);
	//	for (int i = 0;i < numOfTagNodes;i++)
	//	{
	//		result->tagNodes.push_back(ReadTagNode());
	//	}

	//	// materials
	//	result->materials.reserve(numOfMaterials);
	//	for (int i = 0;i < numOfMaterials;i++)
	//	{
	//		result->materials.push_back(ReadMaterial());
	//	}

	//	// meshes
	//	result->meshes.reserve(numOfMeshes);
	//	for (int i = 0;i < numOfMeshes;i++)
	//	{
	//		result->meshes.push_back(ReadMesh());
	//	}
	//	
	//	return result;
	//}
}

