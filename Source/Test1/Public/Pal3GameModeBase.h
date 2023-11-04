// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Pal3GameModeBase.generated.h"

namespace pal3
{
	class Game;
}

/**
 * 
 */
UCLASS()
class TEST1_API APal3GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "pal3")
	void CreateGame();

	UFUNCTION(BlueprintCallable, Category = "pal3")
	void DestroyGame();

	UFUNCTION(BlueprintCallable, Category = "pal3")
	void DebugShowPaths();
};
