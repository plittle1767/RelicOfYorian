// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RY_PlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class UUserWidget;

UCLASS()
class YORIAN_LITTLEPRESTON_API ARY_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

	/** Button callbacks (callable from the widget) */
	UFUNCTION(BlueprintCallable, Category="UI")
	void ResumeGame();

	UFUNCTION(BlueprintCallable, Category="UI")
	void QuitToMenu();

	UFUNCTION(BlueprintCallable, Category="UI")
	void SaveGame();

protected:
	/** the Input Mapping Context to use. You will assign IMC_Default to this in your Player Controller Blueprint. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	/** The Input Action for pausing the game. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	TObjectPtr<UInputAction> PauseAction;

	/** Widget class for the pause menu (assign WBP_PauseMenu in BP_PlayerController) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI")
	TSubclassOf<UUserWidget> PauseMenuClass;

	/** Live instance (created at runtime) */
	UPROPERTY()
	TObjectPtr<UUserWidget> PauseMenu = nullptr;

	/** Toggle from key press (P) */
	void TogglePauseMenu();

	/** Show/Hide internals */
	UFUNCTION(BlueprintCallable, Category="UI")
	void ShowPauseMenu();

	UFUNCTION(BlueprintCallable, Category="UI")
	void HidePauseMenu();
};
