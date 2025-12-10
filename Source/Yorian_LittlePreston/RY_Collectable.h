// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RY_Interactable.h"
#include "GameFramework/Actor.h"
#include "RY_Collectable.generated.h"

UCLASS()
class YORIAN_LITTLEPRESTON_API ARY_Collectable : public AActor, public IRY_Interactable
{
	GENERATED_BODY()

public:
	virtual void Interact_Implementation(AActor* Caller) override;

	// Sets default values for this actor's properties
	ARY_Collectable();

protected:
	// Components
	UPROPERTY(VisibleAnywhere, Category=NPC)
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere, Category=NPC)
	TObjectPtr<UStaticMeshComponent> NPC;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Collectable")
	FName ItemID = "DefaultItem";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Collectable")
	int32 Quantity = 1;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
