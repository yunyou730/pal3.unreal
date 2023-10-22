// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor.h"
#include "Core/Public/Logging/LogMacros.h"
#include "Core/Public/Misc/FileHelper.h"
#include "Core/Public/Misc/Paths.h"

#include "Pal3Core/headers/CpkFileSystem.h"
#include "Pal3Core/headers/cpk/CpkArchive.h"
#include "Pal3Core/headers/mv3/Mv3Reader.h"
#include "Pal3Core/headers/mv3/Mv3.h"
#include "Pal3Core/headers/textureloader/RawTexture.h"
#include "Pal3Core/headers/textureloader/TGALoader.h"

#include "Pal3Ext/GameBoxConverter.h"

#include "ImageUtils.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Misc/FileHelper.h"

#include "UObject/Class.h"

static pal3::CpkFileSystem cpkFileSys;

// Sets default values
AMyActor::AMyActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_rootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("EmptyComponent"));
	SetRootComponent(_rootComponent);


	ConstructorHelpers::FObjectFinder<UMaterialInterface> finder(TEXT("/Game/Test/TestMat"));
	if (finder.Succeeded())
	{
		_materialSample = finder.Object;
	}
	
	_mv3 = ReadMv3();
	//CreateMesh();

	PreCreateSubMeshes();
}


// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//UE_LOG(LogTemp, Warning, TEXT("ayyTick3"));

}

pal3::Mv3* AMyActor::ReadMv3()
{
	auto archive = cpkFileSys.Mount("D:\\code\\pal3_dev\\pal3\\basedata\\basedata.cpk");
	uint32_t len;
	uint8_t* data = archive->GetFileBytesFromFile("ROLE\\104\\C09.MV3", len);

	pal3::Mv3Reader reader(data, len);
	pal3::Mv3* mv3 = reader.Read();

	return mv3;
}

//void AMyActor::CreateMesh()
//{
//	UE_LOG(LogTemp, Warning, TEXT("AMyActor::CreateMesh"));
//	if (_mv3 != nullptr)
//	{
//		InitMeshes(_mv3);
//	}
//}

void AMyActor::PreCreateSubMeshes()
{
	for (size_t i = 0;i < _mv3->meshes.size();i++)
	{
		auto ProceduralMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Procedural Mesh"));
		ProceduralMeshComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		_proceduralMeshes.Push(ProceduralMeshComponent);
	}
}

UProceduralMeshComponent* AMyActor::GetSubMeshAtIndex(int32 index)
{
	return _proceduralMeshes[index];
}

void AMyActor::UpdateMeshes()
{
	UTexture2D* texture = LoadTexture();

	for (size_t subMeshIndex = 0;subMeshIndex < _mv3->meshes.size();subMeshIndex++)
	{	
		UProceduralMeshComponent* proceduralMesh = GetSubMeshAtIndex(subMeshIndex);
		UpdateSubMesh(_mv3, proceduralMesh,subMeshIndex);

		// Setup material
		UMaterialInstanceDynamic* materialInstance = UMaterialInstanceDynamic::Create(_materialSample, GetTransientPackage());
		// setup texture
		materialInstance->SetTextureParameterValue(FName("TexParam"), texture);

		// assign material to mesh 
		proceduralMesh->SetMaterial(0, materialInstance);
	}
}

void AMyActor::UpdateSubMesh(pal3::Mv3* mv3, UProceduralMeshComponent* proceduralMesh,int subMeshIndex)
{
	int frameIndex = 0;

	int32 vertCount = mv3->meshes[subMeshIndex]->keyFrames[frameIndex]->gameBoxVertices.size();
	TArray<FVector> vertices;
	TArray<FVector> normals;
	for (auto it : mv3->meshes[subMeshIndex]->keyFrames[frameIndex]->gameBoxVertices)
	{
		vertices.Add(FVector(it.x, it.y, it.z));
		normals.Add(FVector(0, 0, 1));		// @miao @temp; // normal we should calculate it 
	}

	TArray<int32> triangles;
	//for (auto it : mv3->meshes[0]->gameboxTriangles)
	//{
	//	triangles.Add(it);
	//}

	// @miao @todo
	// try to adjust triangle index
	uint32 triangleCount = mv3->meshes[0]->gameboxTriangles.size() / 3;
	for (uint32 triangleIdx = 0;triangleIdx < triangleCount;triangleIdx++)
	{
		uint32 idx1 = triangleIdx * 3;
		uint32 idx2 = idx1 + 2;
		uint32 idx3 = idx1 + 1;

		triangles.Add(mv3->meshes[0]->gameboxTriangles[idx1]);
		triangles.Add(mv3->meshes[0]->gameboxTriangles[idx2]);
		triangles.Add(mv3->meshes[0]->gameboxTriangles[idx3]);
	}

	TArray<FVector2D> uv0;
	for (auto it : mv3->meshes[0]->UVs)
	{
		uv0.Add(FVector2D(it.x, it.y));
	}

	pal3ext::GameBoxConverter::ConvertVertices(vertices);

	proceduralMesh->CreateMeshSection(0,
		vertices,
		triangles,
		normals,
		uv0,
		TArray<FColor>(),
		TArray<FProcMeshTangent>(),
		false);
}

UTexture2D* AMyActor::LoadTexture()
{
	auto archive = cpkFileSys.GetArchive("D:\\code\\pal3_dev\\pal3\\basedata\\basedata.cpk");
	
	uint32_t len;
	uint8_t* data = archive->GetFileBytesFromFile("ROLE\\104\\104.tga", len);

	pal3::TGALoader loader;
	pal3::RawTexture* rawTexture = loader.Load(data,len);

	// Create Engine Texture Object
	//UTexture2D* texture = NewObject<UTexture2D>(GetTransientPackage(), FName("AyyTexture"), RF_Transient);
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
	texture->PlatformData->Mips.Add(mip);
	texture->UpdateResource();
	

	// Save to HardDisk
	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);
	if (!ImageWrapper.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create image wrapper."));
		return nullptr;
	}
	ImageWrapper->SetRaw(rawTexture->bytes.data(), 
		rawTexture->bytes.size(),
		//rawTexture->width * rawTexture->height * sizeof(FColor), 
		rawTexture->width, rawTexture->height,
		ERGBFormat::RGBA, 
		8);
	TArray<uint8, TSizedDefaultAllocator<64>> CompressedData = ImageWrapper->GetCompressed();

	FString FilePath("D:\\code\\pal3_dev\\test.png");
	if (FFileHelper::SaveArrayToFile(CompressedData, *FilePath))
	{
		UE_LOG(LogTemp, Log, TEXT("Texture saved to: %s"), *FilePath);
	}

	return texture;
}
