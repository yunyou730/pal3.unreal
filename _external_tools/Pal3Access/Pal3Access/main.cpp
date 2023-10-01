#include <cstdio>
#include "headers/cpk/CrcHash.h"
#include "headers/cpk/CpkArchive.h"
#include <cstdint>

#include "headers/CpkFileSystem.h"

#include "headers/mv3/Mv3Reader.h"


void CpkArchiveTestCase()
{
	auto crc = new pal3::Crc32Hash();
	crc->Init();
	{
		pal3::CpkArchive archive("D:\\code\\pal3_dev\\pal3\\basedata\\basedata.cpk", crc, 936);
		archive.Init();

		//uint32_t len;
		//bool bNeedDealloc = false;
		//uint8_t* testData = archive.GetFileBytesFromFile("ROLE\\101\\C01.MV3",len, bNeedDealloc);

		archive.LoadArchiveIntoMemory();
		archive.ExtractTo("D:\\code\\pal3_dev\\testextr");
	}
	delete crc;
}

int main()
{
	printf("pal3 access\n");
	//CpkArchiveTestCase();

	pal3::CpkFileSystem cpkFileSys;
	auto archive = cpkFileSys.Mount("D:\\code\\pal3_dev\\pal3\\basedata\\basedata.cpk");


	uint32_t len;
	uint8_t* data = archive->GetFileBytesFromFile("ROLE\\101\\C01.MV3",len);

	pal3::Mv3Reader reader(data,len);
	pal3::Mv3* mv3 = reader.Read();

	printf("p2\n");
	return 0;
}


