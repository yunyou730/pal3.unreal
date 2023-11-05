#pragma once
#include <string>
#include <vector>
#include "FileLocator.h"

namespace pal3
{
	class FileFinder
	{
	public:
		std::vector<pal3::FileLocator> GetAllRolesFilePath();
	};
}
