#pragma once

#include "CoreMinimal.h"

#include "Core/Public/Logging/LogMacros.h"
#include "Core/Public/Misc/FileHelper.h"
#include "Core/Public/Misc/Paths.h"
#include "Core/Public/Internationalization/Text.h"

#include "Pal3Core/headers/CpkFileSystem.h"
#include "Pal3Core/headers/cpk/CpkArchive.h"
#include "Pal3Core/headers/mv3/Mv3Reader.h"
#include "Pal3Core/headers/mv3/Mv3.h"
#include "Pal3Core/headers/textureloader/RawTexture.h"
#include "Pal3Core/headers/textureloader/TGALoader.h"

#include "ImageUtils.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Misc/FileHelper.h"

namespace pal3
{
	class RenderUtils
	{
	public:
		static UTexture2D* CreateTexture(pal3::RawTexture* rawTexture);
		static bool SaveTextureToDisk(const std::string& destPath,pal3::RawTexture* rawTexture);
	};
}
