// Fill out your copyright notice in the Description page of Project Settings.


#include "RY_Collectable.h"
#include "Engine/Engine.h"
#include "RY_Character.h"

// Sets default values
ARY_Collectable::ARY_Collectable()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ARY_Collectable::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame 
void ARY_Collectable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ARY_Collectable::Interact_Implementation(AActor* Caller)
{
	// Try to see if the thing that interacted is your player character
	if (ARY_Character* PlayerChar = Cast<ARY_Character>(Caller))
	{
		const bool bAdded = PlayerChar->AddItemToInventory(ItemID, GetClass(), Quantity);

		if (bAdded)
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(
					-1,
					1.5f,
					FColor::Green,
					FString::Printf(TEXT("Picked up: %s x%d"), *ItemID.ToString(), Quantity)
				);
			}

			// we can destroy since we picked it up
			Destroy();
			return;
		}
	}

	// fallback
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.2f, FColor::Red, TEXT("Could not add item to inventory"));
	}
}
