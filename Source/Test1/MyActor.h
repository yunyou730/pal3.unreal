// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CustomMeshComponent.h"
#include "ProceduralMeshComponent.h"
#include "StaticMeshOperations.h"
#include "Pal3Ext/Mv3Wrapper.h"

#include "MyActor.generated.h"


UCLASS()
class TEST1_API AMyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AMyActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable, Category = "pal3")
	void SetSpawnParam(FString cpkName,FString dirName,FString fileName);

	UFUNCTION(BlueprintCallable, Category = "pal3")
	void SetFrameSpeed(int32 framePlaySpeed);

	UFUNCTION(BlueprintCallable, Category = "pal3")
	void LoadAndCreateMesh();

	UFUNCTION(BlueprintCallable, Category = "pal3")
	void UpdateMeshes();

private:
	void CreateSubMesh(int subMeshIndex);
	UMaterialInstanceDynamic* CreateSubMeshMaterial(int subMeshIndex);
	void UpdateSubMesh(int subMeshIndex);

protected:
	pal3::Mv3Wrapper* _mv3Wrapper = nullptr;
	USceneComponent* _rootComponent = nullptr;
	UMaterialInterface* _materialSample = nullptr;
	
	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent* _proceduralMesh;

	uint32_t	_frameIndex = 0;
	float	_frameElapsedTime = 0;
	float	_frameKeepTime = 0;

public:
	UPROPERTY(EditAnywhere, Category = "Pal3")
	int32 _framePlaySpeed = 3;

	UPROPERTY(EditAnywhere, Category = "Pal3")
	FString _cpkName;

	UPROPERTY(EditAnywhere, Category = "Pal3")
	FString _dirName;

	UPROPERTY(EditAnywhere, Category = "Pal3")
	FString _fileName;
};
