#pragma once
#include "CoreMinimal.h"

namespace pal3ext
{
	class GameBoxConverter
	{
	public:
		static void ConvertVertices(TArray<FVector>& vertices);
		static void ConvertVertex(FVector& vertex);
	};
}
