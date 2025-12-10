// Fill out your copyright notice in the Description page of Project Settings.


#include "RY_InteractionComponent.h"
#include "RY_Interactable.h"

// Sets default values for this component's properties
URY_InteractionComponent::URY_InteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URY_InteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void URY_InteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void URY_InteractionComponent::LineTraceInteract()
{
	// Get the owner that interacts with this component
	AActor* InteractOwner = GetOwner();

	// Use the Owner's eye location and rotation for line tracing
	FVector EyeLocation;
	FRotator EyeRotation;
	InteractOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	const FVector EndOfLineTrace = EyeLocation + EyeRotation.Vector() * TraceDistance;

	// Perform line tracing
	FHitResult HitResult;
	FCollisionQueryParams CollisionQueryParams(SCENE_QUERY_STAT(InteractTrace), false, InteractOwner);
	const bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, EyeLocation, EndOfLineTrace, TraceChannel,
	                                                       CollisionQueryParams);

	if (bDrawDebug)
	{
		DrawDebugLine(GetWorld(), EyeLocation, EndOfLineTrace, bHit ? FColor::Green : FColor::Red, false, 1.f, 0, 1.5f);
		if (bHit)
		{
			DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 8.f, FColor::Yellow, false, 1.f);
		}
	}

	if (!bHit) return;

	if (AActor* Target = HitResult.GetActor())
	{
		if (Target->GetClass()->ImplementsInterface(URY_Interactable::StaticClass()))
		{
			APawn* OwnerAsPawn = Cast<APawn>(InteractOwner);
			IRY_Interactable::Execute_Interact(Target, OwnerAsPawn);
		}
	}
}
