#include "Settings.h"
#include "Core/Public/Logging/LogMacros.h"
#include "Core/Public/Misc/Paths.h"
#include "Core/Public/CoreTypes.h"

static const std::string kPal3RawResDir = "pal3_raw_res";
static const std::string kDirectorySeperator = "\\";

namespace pal3
{
	Settings::Settings()
	{
		
	}

	Settings::~Settings()
	{
	
	}

	void Settings::Initialize()
	{
		std::string projContentDir = TCHAR_TO_UTF8(*FPaths::ProjectContentDir());
		_resourceRootPath = projContentDir + kPal3RawResDir + kDirectorySeperator;
		//UE_LOG(LogTemp,Warning,TEXT("[_resourcerootpath]%s"),*_resourceRootPath);
	}

	std::string Settings::GetResourceRoot()
	{
		return _resourceRootPath;
	}
}
