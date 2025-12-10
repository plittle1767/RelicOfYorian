// Fill out your copyright notice in the Description page of Project Settings.


#include "RY_NPC.h"
#include "Engine/Engine.h"

// Sets default values
ARY_NPC::ARY_NPC()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ARY_NPC::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ARY_NPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARY_NPC::Interact_Implementation(AActor* Caller)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan,
		                                 TEXT("Ahhh, may chaos take the world! May chaos take the world!"));
	}
}
