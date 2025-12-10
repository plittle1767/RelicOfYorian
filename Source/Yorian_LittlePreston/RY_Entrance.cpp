// Fill out your copyright notice in the Description page of Project Settings.


#include "RY_Entrance.h"

// Sets default values
ARY_Entrance::ARY_Entrance()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root.Get());

	EntranceFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EntranceFrame"));
	EntranceFrame->SetupAttachment(Root.Get());
	EntranceFrame->SetCollisionProfileName(TEXT("BlockAll"));

	Entrance = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Entrance"));
	Entrance->SetupAttachment(Root.Get());
	Entrance->SetCollisionProfileName(TEXT("BlockAll"));

	EntranceTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("EntranceTimeline"));
}

// Called when the game starts or when spawned
void ARY_Entrance::BeginPlay()
{
	Super::BeginPlay();

	if (TimelineCurve)
	{
		FOnTimelineFloat Update;
		Update.BindUFunction(this, FName("OnTimelineUpdate"));
		EntranceTimeline->AddInterpFloat(TimelineCurve.Get(), Update);
		EntranceTimeline->SetLooping(false);
	}
}

// Called every frame
void ARY_Entrance::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARY_Entrance::OnTimelineUpdate(float StartingYaw)
{
	const float EntranceYaw = YawOffset + StartingYaw;
	Entrance->SetRelativeRotation(FRotator(0.f, EntranceYaw, 0.f));
}

void ARY_Entrance::Interact_Implementation(AActor* Caller)
{
	if (!EntranceTimeline || !TimelineCurve)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 1.2f, FColor::Red, TEXT("EntranceTimeline/Curve missing"));
		return;
	}

	if (bFromStart)
	{
		EntranceTimeline->PlayFromStart(); // BP FlipFlop -> A pin
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Entrance opening"));
	}

	else
	{
		EntranceTimeline->ReverseFromEnd(); // BP FlipFlop -> B pin
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange, TEXT("Entrance closing"));
	}

	bFromStart = !bFromStart;
}

