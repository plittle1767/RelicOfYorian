// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RY_Interactable.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "RY_Entrance.generated.h"

UCLASS()
class YORIAN_LITTLEPRESTON_API ARY_Entrance : public AActor, public IRY_Interactable
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	float MaxYaw;
	
	void Interact_Implementation(AActor* Caller) override;
	
	// Sets default values for this actor's properties
	ARY_Entrance();

protected:
	// Components
	UPROPERTY(VisibleAnywhere, Category="Entrance")
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere, Category="Entrance")
	TObjectPtr<UStaticMeshComponent> EntranceFrame;

	UPROPERTY(VisibleAnywhere, Category="Entrance")
	TObjectPtr<UStaticMeshComponent> Entrance;

	UPROPERTY(VisibleAnywhere, Category="Entrance")
	float YawOffset = 0.f;

	// Timeline
	UPROPERTY(VisibleAnywhere, Category="Entrance|Timeline")
	TObjectPtr<UTimelineComponent> EntranceTimeline;

	UPROPERTY(EditAnywhere, Category="Entrance|Timeline")
	TObjectPtr<UCurveFloat> TimelineCurve = nullptr;

	UFUNCTION()
	void OnTimelineUpdate(float StartingYaw);

	// Rotate from starting (open) or reverse (close)
	bool bFromStart = true;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
