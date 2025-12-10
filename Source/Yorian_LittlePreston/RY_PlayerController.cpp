// Fill out your copyright notice in the Description page of Project Settings.


#include "RY_PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void ARY_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Get the Enhanced Input Local Player Subsystem from the Local Player.
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer()))
	{
		// Add the mapping context.
		if (DefaultMappingContext)
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	if (PauseMenuClass)
	{
		PauseMenu = CreateWidget<UUserWidget>(this, PauseMenuClass);
	}
}

void ARY_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (PauseAction)
		{
			// One key to open/close the menu
			EIC->BindAction(PauseAction, ETriggerEvent::Started, this, &ARY_PlayerController::TogglePauseMenu);
		}
	}
}

void ARY_PlayerController::TogglePauseMenu()
{
	const bool bIsPaused = UGameplayStatics::IsGamePaused(GetWorld());
	if (bIsPaused)
	{
		HidePauseMenu();
	}
	else
	{
		ShowPauseMenu();
	}
}

void ARY_PlayerController::ShowPauseMenu()
{
	if (!PauseMenu && PauseMenuClass)
	{
		PauseMenu = CreateWidget<UUserWidget>(this, PauseMenuClass);
	}
	if (!PauseMenu) return;

	if (!PauseMenu->IsInViewport())
	{
		PauseMenu->AddToViewport(100);
	}

	UGameplayStatics::SetGamePaused(GetWorld(), true);

	FInputModeUIOnly UIOnly;
	UIOnly.SetWidgetToFocus(PauseMenu->TakeWidget());
	UIOnly.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(UIOnly);

	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
}

void ARY_PlayerController::HidePauseMenu()
{
	if (PauseMenu && PauseMenu->IsInViewport())
	{
		PauseMenu->RemoveFromParent();
	}

	UGameplayStatics::SetGamePaused(GetWorld(), false);

	FInputModeGameOnly GameOnly;
	SetInputMode(GameOnly);

	bShowMouseCursor = false;
	bEnableClickEvents = false;
	bEnableMouseOverEvents = false;
}

void ARY_PlayerController::ResumeGame()
{
	HidePauseMenu();
}

void ARY_PlayerController::QuitToMenu()
{
	HidePauseMenu();
	// TODO: Create Main Menu "map" and replace "MainMenu" with correct path
	// UGameplayStatics::OpenLevel(this, FName(TEXT("MainMenu")));
}

void ARY_PlayerController::SaveGame()
{
	// In the future, when a SaveGame class has been created, hook it up here.
	// For now, just display a message saying that the game has been saved.
	GEngine->AddOnScreenDebugMessage(-1, 1.2f, FColor::Green, TEXT("Gave has been saved."));
}
