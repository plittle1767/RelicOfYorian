// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RY_Interactable.h"
#include "RY_Letter.generated.h"

class UStaticMeshComponent;
class UUserWidget;

UCLASS()
class YORIAN_LITTLEPRESTON_API ARY_Letter : public AActor, public IRY_Interactable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARY_Letter();

	virtual void Interact_Implementation(AActor* Caller) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category="Letter")
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere, Category="Letter")
	TObjectPtr<UStaticMeshComponent> LetterMesh;

	// What to show when the player reads it
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Letter")
	FText LetterTitle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Letter", meta=(MultiLine="true"))
	FText LetterBody;

	// Widget class to show on screen
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Letter|UI")
	TSubclassOf<UUserWidget> LetterWidgetClass;

	// Should the letter disappear after reading?
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Letter")
	bool bDestroyAfterRead = false;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
