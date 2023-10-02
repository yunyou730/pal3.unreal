// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor.h"
#include "Core/Public/Logging/LogMacros.h"
#include "Core/Public/Misc/FileHelper.h"
#include "Core/Public/Misc/Paths.h"

#include "Pal3/headers/CpkFileSystem.h"
#include "Pal3/headers/cpk/CpkArchive.h"
#include "Pal3/headers/mv3/Mv3Reader.h"
#include "Pal3/headers/mv3/Mv3.h"

// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	UE_LOG(LogTemp, Warning, TEXT("ayy"));


	ProceduralMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("MyProceduralComponent"));;
	RootComponent = ProceduralMeshComponent;
}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("ayy3"));

	FString rootDir = FPaths::RootDir();
	UE_LOG(LogTemp, Warning, TEXT("Root dir %s"),*rootDir);


	UE_LOG(LogTemp, Warning, TEXT("Root dir %s"), *FPaths::ProjectContentDir());
	

	TArray<uint8> bin;
	FFileHelper::LoadFileToArray(bin, *FString(""));

	pal3::CpkFileSystem cpkFileSys;
	auto archive = cpkFileSys.Mount("D:\\code\\pal3_dev\\pal3\\basedata\\basedata.cpk");

	uint32_t len;
	uint8_t* data = archive->GetFileBytesFromFile("ROLE\\104\\C09.MV3", len);

	pal3::Mv3Reader reader(data, len);
	pal3::Mv3* mv3 = reader.Read();

	//pal3::Crc32Hash crcHash;
	//crcHash.Init();
	//crcHash.Compute("cbdata\\pal3_softstar.gdb", true);

	//pal3::CpkArchive* cpk = new pal3::CpkArchive("D:\\code\\pal3_dev\\pal3\\basedata\\basedata.cpk",&crcHash,936);
	//cpk->Init();
	UE_LOG(LogTemp, Warning, TEXT("ayy4"));

	float kScale = 0.1f;
	int keyIndex = 0;

	int32 vertCount = mv3->meshes[0]->keyFrames[keyIndex]->gameBoxVertices.size();
	TArray<FVector> vertices;
	TArray<FVector> normals;
	for(auto it : mv3->meshes[0]->keyFrames[keyIndex]->gameBoxVertices)
	{
		vertices.Add(FVector(it.x, it.y, it.z) * kScale);
		normals.Add(FVector(0, 0, 1));
	}

	TArray<int32> triangles;
	for (auto it : mv3->meshes[0]->gameboxTriangles)
	{
		triangles.Add(it);
	}

	//TArray<FVector> normals;
	//normals.Add(FVector(0, 0, 1));
	//normals.Add(FVector(0, 0, 1));
	//normals.Add(FVector(0, 0, 1));

	TArray<FVector2D> uv0;
	for (auto it : mv3->meshes[0]->UVs)
	{
		uv0.Add(FVector2D(it.x, it.y));
	}

	ProceduralMeshComponent->CreateMeshSection(
		0, 
		vertices, 
		triangles, 
		normals, 
		uv0, 
		TArray<FColor>(), 
		TArray<FProcMeshTangent>(), 
		false);

}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UE_LOG(LogTemp, Warning, TEXT("ayyTick222"));

}

