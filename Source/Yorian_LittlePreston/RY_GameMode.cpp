// Fill out your copyright notice in the Description page of Project Settings.


#include "RY_GameMode.h"
#include "RY_GameState.h"
#include "RY_PlayerState.h"

ARY_GameMode::ARY_GameMode()
{
	GameStateClass = ARY_GameState::StaticClass();

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(
		TEXT("/Game/Blueprints/BP_PlayerController"));

	if (PlayerControllerBPClass.Class != nullptr)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}

	PlayerStateClass = ARY_PlayerState::StaticClass();
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_Character"));

	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
