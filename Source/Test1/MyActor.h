// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CustomMeshComponent.h"
#include "ProceduralMeshComponent.h"
#include "StaticMeshOperations.h"

#include "MyActor.generated.h"

namespace pal3 {
	struct Mv3;
}

UCLASS()
class TEST1_API AMyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	//UFUNCTION(BlueprintCallable,Category = "pal3")
	//void CreateMesh();

	//UFUNCTION(BlueprintCallable, Category = "pal3")
	//void ReplaceTexture();

	UFUNCTION(BlueprintCallable, Category = "pal3")
	void UpdateMeshes();

private:
	pal3::Mv3* ReadMv3();
	UTexture2D* LoadTexture();
	void UpdateSubMesh(pal3::Mv3* mv3, UProceduralMeshComponent* proceduralMesh,int subMeshIndex);

private:
	void PreCreateSubMeshes();
	UProceduralMeshComponent* GetSubMeshAtIndex(int32 index);

protected:
	pal3::Mv3* _mv3 = nullptr;
	USceneComponent* _rootComponent = nullptr;
	UMaterialInterface* _materialSample = nullptr;

	UPROPERTY(VisibleAnywhere)
	TArray<UProceduralMeshComponent*>	_proceduralMeshes;
};
