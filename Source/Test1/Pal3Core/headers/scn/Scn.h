#pragma once
#include <cstdint>
#include "../GameBoxType.h"
#include <vector>

namespace pal3
{
	struct ScnInfo;
	struct ScnNpcInfo;
	struct Scn
	{
		//ScnInfo sceneInfo;
		

	};

	typedef enum
	{
		OutDoor = 0,
		InDoor,
		Maze,
	} EScnType;

	typedef enum
	{
		StoryNpc = 0,
		HotelManager = 1,
		CombatNpc = 2,
		Dealer = 3,
		Soldier = 4,
		MainActor = 0xff,
	} EActorType;

	typedef enum
	{
		None = 0,
		Wander,
		PathFollow,
		Hold,

		// blow not used
		Seek,
		Pursuit,
		Evasion,
		PathLayer,
		MoveBack,
		NpcPathFollow,
		PetFollow,
		PetFly,
		Key,
	} EActorBehaviourType;

	struct ScnInfo
	{
	public:
		char cityName[32];	// region name 
		char sceneName[32];	// scene name
		char modelName[32];	// model name
		EScnType scnType;
		int32_t lightMap;	// 0: day light, 1: night light, -1: no light
		uint32_t skyBox;
		int32_t reserved[6];	// 6x32 bits, unused
	};

	struct ScnNpcInfo
	{
		uint8_t id;
		EActorType actorType;
		char name[32];

		char texture[34];	// unused. texture id

		float facingDirection;	// actor initial face

		int32_t layerIndex;	// initial layer index

		// initial position in GameBox (X,Z)
		float gameboxPositionX;
		float gameboxPositionZ;

		int32_t initActive;

		EActorBehaviourType initBehaviour;

		uint32_t scriptId;

		float gameboxYPosition;	// only when InitBehaviour == Hold, gameboxYPositon & initAction works
		char initAction[16];	// initial action



	};
}
