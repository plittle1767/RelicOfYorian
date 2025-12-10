// Fill out your copyright notice in the Description page of Project Settings.


#include "RY_PauseMenu.h"
#include "Components/Button.h"
#include "RY_PlayerController.h"

void URY_PauseMenu::NativeConstruct()
{
	Super::NativeConstruct();

	if (ResumeButton)
	{
		ResumeButton->OnClicked.AddDynamic(this, &URY_PauseMenu::OnResumeClicked);
	}
	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &URY_PauseMenu::OnQuitClicked);
	}
	if (SaveButton)
	{
		SaveButton->OnClicked.AddDynamic(this, &URY_PauseMenu::OnSaveClicked);
	}
}

void URY_PauseMenu::NativeDestruct()
{
	Super::NativeDestruct();
}

void URY_PauseMenu::OnResumeClicked()
{
	if (APlayerController* Owner = GetOwningPlayer())
	{
		if (auto* RYPC = Cast<ARY_PlayerController>(Owner))
		{
			RYPC->ResumeGame();
		}
	}
}

void URY_PauseMenu::OnQuitClicked()
{
	if (APlayerController* Owner = GetOwningPlayer())
	{
		if (auto* RYPC = Cast<ARY_PlayerController>(Owner))
		{
			RYPC->QuitToMenu();
		}
	}
}

void URY_PauseMenu::OnSaveClicked()
{
	if (APlayerController* Owner = GetOwningPlayer())
	{
		if (auto* RYPC = Cast<ARY_PlayerController>(Owner))
		{
			RYPC->SaveGame();
		}
	}
}

