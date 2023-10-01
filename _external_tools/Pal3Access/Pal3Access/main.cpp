#include <cstdio>
#include "headers/CrcHash.h"
#include "headers/CpkArchive.h"
#include <cstdint>

int main()
{
	printf("pal3 access\n");
	
	auto crc = new pal3::Crc32Hash();
	crc->Init();

	{
		printf("p1\n");
		pal3::CpkArchive archive("D:\\code\\pal3_dev\\pal3\\basedata\\basedata.cpk", crc, 936);
		archive.Init();

		//uint32_t len;
		//bool bNeedDealloc = false;
		//uint8_t* testData = archive.GetFileBytesFromFile("ROLE\\101\\C01.MV3",len, bNeedDealloc);

		archive.LoadArchiveIntoMemory();
		archive.ExtractTo("D:\\code\\pal3_dev\\testextr");
		
	}
	delete crc;
	
	printf("p2\n");
	return 0;
}


