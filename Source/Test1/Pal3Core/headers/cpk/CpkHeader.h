#include <cstdint>
namespace pal3
{
#pragma pack(push,1)
	struct CpkHeader
	{
		uint32_t label;
		uint32_t version;
		uint32_t tableStart;
		uint32_t dataStart;
		uint32_t maxFileNum;
		uint32_t fileNum;
		uint32_t isFormated;
		uint32_t sizeOfheader;
		uint32_t validTableNum;
		uint32_t maxTableNum;
		uint32_t fragmentNum;	// fragmentNum = validTableNum - fileNum
		uint32_t packageSize;
		uint32_t reserved[20];
	};
#pragma pack(pop)
}
