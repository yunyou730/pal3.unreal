// Fill out your copyright notice in the Description page of Project Settings.

#include "Pal3GameModeBase.h"
#include "Core/Public/Logging/LogMacros.h"

#include "../Pal3Game/Game.h"

void APal3GameModeBase::CreateGame()
{
	UE_LOG(LogTemp, Warning, TEXT("APal3GameModeBase::CreateGame"));
}

void APal3GameModeBase::DestroyGame()
{
	UE_LOG(LogTemp, Warning, TEXT("APal3GameModeBase::DestroyGame"));
	pal3::Game::Release();
}

void APal3GameModeBase::DebugShowPaths()
{
	//UE_LOG(LogTemp, Warning, TEXT("My string: %s"), *myString);

	UE_LOG(LogTemp, Warning, TEXT("[ProjectContentDir]:%s"), *FPaths::ProjectContentDir());
	UE_LOG(LogTemp, Warning, TEXT("[ProjectDir]:%s"), *FPaths::ProjectDir());
}
