// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "RY_InventoryComponent.generated.h"

// Simple item struct that can expand later
USTRUCT(BlueprintType)
struct FRY_InventoryItem
{
	GENERATED_BODY()

	// What is this item?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FName ItemID;

	// How many of it do we have?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	int32 Quantity = 1;

	// (Optional) keep a pointer to the class we picked up
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	TSubclassOf<AActor> SourceClass;
};


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class YORIAN_LITTLEPRESTON_API URY_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URY_InventoryComponent();

	// Expose this so you can look at it in BP
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory")
	TArray<FRY_InventoryItem> Items;

	// Try to add an item. If it already exists, increase quantity.
	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool AddItem(const FName& ItemID, TSubclassOf<AActor> SourceClass, int32 Amount = 1);

	// For UI or debugging
	UFUNCTION(BlueprintCallable, Category="Inventory")
	const TArray<FRY_InventoryItem>& GetItems() const { return Items; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
