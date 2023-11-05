// Fill out your copyright notice in the Description page of Project Settings.

#include "MyActor.h"
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

#include "Pal3Ext/GameBoxConverter.h"

#include "ImageUtils.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Misc/FileHelper.h"

#include "UObject/Class.h"

#include "Pal3Game/Game.h"
#include "Pal3Game/Settings.h"
#include "Pal3Game/RawAssetCache.h"
#include "Pal3Game/RenderUtils.h"

// Sets default values
AMyActor::AMyActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Root Component
	_rootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("EmptyComponent"));
	SetRootComponent(_rootComponent);

	// Hold Sample Material 
	ConstructorHelpers::FObjectFinder<UMaterialInterface> finder(TEXT("/Game/Test/TestMat"));
	if (finder.Succeeded())
	{
		_materialSample = finder.Object;
	}

	// Create ProceduralMeshComponent
	_proceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("pal3_procedural_mesh"));
	_proceduralMesh->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

	// Mv3 Wrapper
	_mv3Wrapper = new pal3::Mv3Wrapper();

	SetFrameSpeed(3);
}

void AMyActor::SetSpawnParam(FString cpkName, FString dirName, FString fileName)
{
	_cpkName = cpkName;
	_dirName = dirName;
	_fileName = fileName;
}

void AMyActor::SetFrameSpeed(int32 framePlaySpeed)
{
	_framePlaySpeed = framePlaySpeed;
	_frameKeepTime = 1.0f / _framePlaySpeed;
	_frameElapsedTime = 0.0f;
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
	
	if (_mv3Wrapper != nullptr && _mv3Wrapper->HasLoaded())
	{
		_frameElapsedTime += DeltaTime;
		if (_frameElapsedTime >= _frameKeepTime)
		{
			_frameElapsedTime -= _frameKeepTime;

			// Process to next frame
			_frameIndex++;
			if (_frameIndex >= _mv3Wrapper->GetFrameCount())
			{
				_frameIndex %= _mv3Wrapper->GetFrameCount();
			}
		}
	}
}

void AMyActor::LoadAndCreateMesh()
{
	//_mv3Wrapper->Init("basedata.cpk", "ROLE\\104", "C09.MV3");
	_mv3Wrapper->Init(TCHAR_TO_UTF8(*_cpkName), TCHAR_TO_UTF8(*_dirName), TCHAR_TO_UTF8(*_fileName));
	
	for (uint32_t subMeshIndex = 0;subMeshIndex < _mv3Wrapper->GetSubMeshCount();subMeshIndex++)
	{
		UMaterialInstanceDynamic* materialInstance = CreateSubMeshMaterial(subMeshIndex);
		_proceduralMesh->SetMaterial(subMeshIndex, materialInstance);
		
		CreateSubMesh(subMeshIndex);
	}
}

void AMyActor::UpdateMeshes()
{
	for (uint32_t subMeshIndex = 0; subMeshIndex < _mv3Wrapper->GetSubMeshCount(); subMeshIndex++)
	{
		UpdateSubMesh(subMeshIndex);
	}
}

void AMyActor::CreateSubMesh(int subMeshIndex)
{
	TArray<FVector> vertices;
	TArray<FVector> normals;
	TArray<int32_t> triangles;
	TArray<FVector2D> uv0;

	_mv3Wrapper->GetVerticesAtFrameIndex(subMeshIndex, _frameIndex, vertices, triangles, normals, uv0);

	_proceduralMesh->CreateMeshSection(subMeshIndex,
				vertices,
				triangles,
				normals,
				uv0,
				TArray<FColor>(),
				TArray<FProcMeshTangent>(),
				false);
}

UMaterialInstanceDynamic* AMyActor::CreateSubMeshMaterial(int subMeshIndex)
{
	UMaterialInstanceDynamic* materialInstance = UMaterialInstanceDynamic::Create(_materialSample, GetTransientPackage());
	pal3::RawTexture* rawTexture = _mv3Wrapper->GetTextureAtMeshIndex(subMeshIndex);
	UTexture2D* texture = pal3::RenderUtils::CreateTexture(rawTexture);
	materialInstance->SetTextureParameterValue(FName("TexParam"), texture);

	return materialInstance;
}

void AMyActor::UpdateSubMesh(int subMeshIndex)
{
	TArray<FVector> vertices;
	TArray<FVector> normals;
	TArray<int32_t> triangles;
	TArray<FVector2D> uv0;

	_mv3Wrapper->GetVerticesAtFrameIndex(subMeshIndex, _frameIndex, vertices, triangles, normals, uv0);

	_proceduralMesh->UpdateMeshSection(
		subMeshIndex,
		vertices,
		normals,
		uv0,
		TArray<FColor>(),
		TArray<FProcMeshTangent>()
	);
}
