#pragma once
#include <string>
#include <vector>

namespace pal3
{
	class FileLocator
	{
	public:
		FileLocator(const std::string& cpkName, const std::string& relativeDirectory, const std::string& fileName)
		{
			_cpkName = cpkName;
			_relativeDirectory = relativeDirectory;
			_fileName = fileName;
		}

		std::string GetFilePathInSameDir(const std::string& fileName)
		{
			return _relativeDirectory + "\\" + fileName;
		}

		std::string _cpkName;
		std::string _relativeDirectory;
		std::string _fileName;
	};
}
