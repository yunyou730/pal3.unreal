#pragma once

#include <string>

namespace pal3
{
	class Settings
	{
	public:
		Settings();
		~Settings();

		void Initialize();

		std::string GetResourceRoot();

	private:
		
	private:
		std::string _resourceRootPath;
	};
}