#pragma once
#include <cstdint>
#include "Mv3.h"

namespace pal3
{
	//struct Mv3;
	class Mv3Reader
	{
	public:
		Mv3Reader(uint8_t* data,uint32_t len);
		Mv3* Read();

	private:
		Mv3AnimationEvent* ReadAnimationEvent();
		Mv3TagNode* ReadTagNode();
		Mv3TagFrame* ReadTagFrame();
		GameBoxMaterial* ReadMaterial();
		Mv3Mesh* ReadMesh();


		void BuildMesh(Mv3Mesh* targetMesh,
			const std::vector<Mv3Attribute*>& attributes,
			const std::vector<Mv3VertFrame*>& vertFrames,
			const std::vector<GameBoxVector2>& texCoords);

	private:
		uint8_t* _data = nullptr;
		uint32_t _len = 0;
		uint32_t _cur = 0;
	};
}
