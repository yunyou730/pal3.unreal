#pragma once
#include <string>
#include <vector>

namespace pal3
{
	struct GameBoxVector3
	{
		float x, y, z;

		GameBoxVector3() :x(0), y(0), z(0)
		{

		}

		GameBoxVector3(float x, float y, float z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}
	};

	struct GameBoxVector2
	{
		float x, y;

		GameBoxVector2() :x(0), y(0)
		{

		}

		GameBoxVector2(float x, float y)
		{
			this->x = x;
			this->y = y;
		}

		GameBoxVector2(const GameBoxVector2& other)
		{
			this->x = other.x;
			this->y = other.y;
		}
	};


	struct GameBoxQuaternion
	{
		float x, y, z, w;

		GameBoxQuaternion()
			:x(0),y(0),z(0),w(0)
		{
		
		}

		GameBoxQuaternion(float x, float y, float z, float w)
		{
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}
	};

	struct GameBoxColor
	{
		float R, G, B, A;

		GameBoxColor()
			:R(0),G(0),B(0),A(0)
		{
		
		}

		GameBoxColor(float r, float g, float b, float a)
		{
			R = r;
			G = g;
			B = b;
			A = a;
		}
	};

	struct GameBoxMaterial
	{
		GameBoxColor diffuse;
		GameBoxColor ambient;
		GameBoxColor specular;
		GameBoxColor emissive;
		float specularPower;
		std::vector<std::string> textureFileNames;
	};
}
