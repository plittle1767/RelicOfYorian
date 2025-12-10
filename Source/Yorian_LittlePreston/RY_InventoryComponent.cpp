// Fill out your copyright notice in the Description page of Project Settings.


#include "RY_InventoryComponent.h"
#include "Engine/Engine.h"

// Sets default values for this component's properties
URY_InventoryComponent::URY_InventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void URY_InventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void URY_InventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool URY_InventoryComponent::AddItem(const FName& ItemID, TSubclassOf<AActor> SourceClass, int32 Amount)
{
	if (ItemID.IsNone() || Amount <= 0)
	{
		return false;
	}

	// See if we already have this item
	for (FRY_InventoryItem& Existing : Items)
	{
		if (Existing.ItemID == ItemID)
		{
			Existing.Quantity += Amount;

			// Debug so you see it’s working
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(
					-1,
					1.5f,
					FColor::Cyan,
					FString::Printf(TEXT("Added to existing item: %s x%d"), *ItemID.ToString(), Amount)
				);
			}
			return true;
		}
	}

	// Otherwise, make a new entry
	FRY_InventoryItem NewItem;
	NewItem.ItemID = ItemID;
	NewItem.Quantity = Amount;
	NewItem.SourceClass = SourceClass;

	Items.Add(NewItem);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			1.5f,
			FColor::Cyan,
			FString::Printf(TEXT("Added NEW item: %s"), *ItemID.ToString())
		);
	}

	return true;
}
