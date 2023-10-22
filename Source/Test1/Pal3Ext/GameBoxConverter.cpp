#include "GameBoxConverter.h"
#include <cstdio>
#include "Math/Quat.h"

namespace pal3ext
{
	void GameBoxConverter::ConvertVertices(TArray<FVector>& vertices)
	{
		for (auto it = vertices.CreateIterator();it;it++)
		{
			FVector& vertex = *it;
			ConvertVertex(vertex);
		}
	}

	void GameBoxConverter::ConvertVertex(FVector& vertex)
	{
		FQuat rotation(FVector(1, 0, 0), FMath::DegreesToRadians(90));
		float scale = 0.05;
		vertex = vertex * scale;
		vertex = rotation.RotateVector(vertex);
	}
}
