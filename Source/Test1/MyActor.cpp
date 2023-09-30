// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor.h"
#include "Core/Public/Logging/LogMacros.h"
#include "Core/Public/Misc/FileHelper.h"
#include "Core/Public/Misc/Paths.h"
#include "Pal3/CpkArchive.h"
#include "Pal3/CrcHash.h"

// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	UE_LOG(LogTemp, Warning, TEXT("ayy"));
}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("ayy2"));

	FString rootDir = FPaths::RootDir();
	UE_LOG(LogTemp, Warning, TEXT("Root dir %s"),*rootDir);


	UE_LOG(LogTemp, Warning, TEXT("Root dir %s"), *FPaths::ProjectContentDir());
	

	TArray<uint8> bin;
	FFileHelper::LoadFileToArray(bin, *FString(""));

	pal3::Crc32Hash crcHash;
	crcHash.Init();
	crcHash.Compute("cbdata\\pal3_softstar.gdb", true);

	pal3::CpkArchive* cpk = new pal3::CpkArchive("D:\\code\\pal3_dev\\pal3\\basedata\\basedata.cpk",&crcHash,936);
	cpk->Init();
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UE_LOG(LogTemp, Warning, TEXT("ayyTick222"));

}

