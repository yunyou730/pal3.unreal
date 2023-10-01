#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace pal3
{
	class CpkEntry
	{
	public:
		CpkEntry(const std::string& virtualPath,bool isDirectory)
			: VirtualPath(virtualPath)
			,IsDirectory(isDirectory)
		{
			
		}

		CpkEntry(const std::string& virtualPath, bool isDirectory,const std::vector<CpkEntry*>& children)
			: VirtualPath(virtualPath)
			, IsDirectory(isDirectory)
			, Children(children)
		{

		}

		std::string VirtualPath;
		bool IsDirectory;
		std::vector<CpkEntry*> Children;
	};
}
