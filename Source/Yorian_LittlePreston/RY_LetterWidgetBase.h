// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RY_LetterWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class YORIAN_LITTLEPRESTON_API URY_LetterWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category="Letter")
	void SetupLetter(const FText& InTitle, const FText& InBody);
};
