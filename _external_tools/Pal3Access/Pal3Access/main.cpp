#include <cstdio>
#include "headers/CrcHash.h"
#include "headers/CpkArchive.h"
#include <cstdint>

int main()
{
	printf("pal3 access\n");
	
	auto crc = new pal3::Crc32Hash();
	crc->Init();

	pal3::CpkArchive archive("D:\\code\\pal3_dev\\pal3\\basedata\\basedata.cpk",crc,936);
	archive.Init();
	archive.LoadArchiveIntoMemory();

	std::string test = "init.sce";
	if (archive.FileExists(test))
	{
		printf("test1\n");
	}
	else
	{
		printf("test2\n");
	}

	test = "caption\\1A.tga";
	if (archive.FileExists(test))
	{
		printf("test1\n");
	}
	else
	{
		printf("test2\n");
	}

	archive.ExtractTo("D:\\code\\pal3_dev\\test_extract");

	return 0;
}


