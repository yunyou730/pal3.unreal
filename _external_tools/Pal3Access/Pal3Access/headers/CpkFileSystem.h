#include <map>
#include <string>

namespace pal3
{
	class CpkArchive;
	class Crc32Hash;
	class CpkFileSystem
	{
	public:
		CpkFileSystem(const std::string& rootPath,Crc32Hash* crcHash);

	private:
		std::map<std::string, CpkArchive*> _cpkArchives;
		Crc32Hash* _crcHash;
		std::string _rootPath;
	};
}
