// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RY_PauseMenu.generated.h"

class UButton;

UCLASS()
class YORIAN_LITTLEPRESTON_API URY_PauseMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	UPROPERTY(meta=(BindWidget))
	UButton* ResumeButton;

	UPROPERTY(meta=(BindWidget))
	UButton* QuitButton;

	UPROPERTY(meta=(BindWidget))
	UButton* SaveButton;

private:
	UFUNCTION()
	void OnResumeClicked();

	UFUNCTION()
	void OnQuitClicked();

	UFUNCTION()
	void OnSaveClicked();
};
