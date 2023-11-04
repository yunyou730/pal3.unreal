#include "RenderUtils.h"

namespace pal3
{
	UTexture2D* RenderUtils::CreateTexture(pal3::RawTexture* rawTexture)
	{
		UTexture2D* texture = NewObject<UTexture2D>(GetTransientPackage());
		texture->PlatformData = new FTexturePlatformData();
		texture->PlatformData->SizeX = rawTexture->width;
		texture->PlatformData->SizeY = rawTexture->height;
		texture->PlatformData->SetNumSlices(1);
		texture->PlatformData->PixelFormat = PF_R8G8B8A8;

		FTexture2DMipMap* mip = new FTexture2DMipMap();
		mip->SizeX = rawTexture->width;
		mip->SizeY = rawTexture->height;

		mip->BulkData.Lock(LOCK_READ_WRITE),
			FMemory::Memcpy(
				mip->BulkData.Realloc(rawTexture->width * rawTexture->height * 4),
				rawTexture->bytes.data(),
				rawTexture->width * rawTexture->height * 4
			);
		mip->BulkData.Unlock();
		//texture->PlatformData->Mips.Add(mip);
		texture->GetPlatformData()->Mips.Add(mip);
		texture->UpdateResource();

		return texture;
	}

	bool RenderUtils::SaveTextureToDisk(const std::string& destPath, pal3::RawTexture* rawTexture)
	{
		IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
		TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);
		if (!ImageWrapper.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create image wrapper."));
			return false;
		}
		ImageWrapper->SetRaw(rawTexture->bytes.data(),
			rawTexture->bytes.size(),
			//rawTexture->width * rawTexture->height * sizeof(FColor), 
			rawTexture->width, rawTexture->height,
			ERGBFormat::RGBA,
			8);
		TArray<uint8, TSizedDefaultAllocator<64>> CompressedData = ImageWrapper->GetCompressed();

		FString FilePath(destPath.c_str());
		if (FFileHelper::SaveArrayToFile(CompressedData, *FilePath))
		{
			UE_LOG(LogTemp, Log, TEXT("Texture saved to: %s"), *FilePath);
		}

		return true;
	}
}
