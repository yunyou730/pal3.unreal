#include <map>
#include <string>

namespace pal3
{
	class CpkArchive;
	class Crc32Hash;
	class CpkFileSystem
	{
	public:
		CpkFileSystem();
		~CpkFileSystem();

		CpkArchive* Mount(const std::string& cpkFilePath);

	private:
		void ReleaseCRCHash();
		void ReleaseMountedArchives();

	private:
		std::map<std::string, CpkArchive*> _cpkArchives;
		Crc32Hash* _crcHash;
		int _codePage;
	};
}
