// Fill out your copyright notice in the Description page of Project Settings.


#include "RY_Letter.h"
#include "RY_LetterWidgetBase.h"
#include "Components/StaticMeshComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "Engine/Engine.h"

// Sets default values
ARY_Letter::ARY_Letter()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	LetterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LetterMesh"));
	LetterMesh->SetupAttachment(Root);

	// default text so you can test
	LetterTitle = FText::FromString(TEXT("Default Letter Title"));
	LetterBody = FText::FromString(TEXT("To [insert name],\n[insert body text\n- [insert name]"));
}

// Called when the game starts or when spawned
void ARY_Letter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ARY_Letter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARY_Letter::Interact_Implementation(AActor* Caller)
{
	// We’re gonna show a widget to the player
	APlayerController* PC = nullptr;

	// If the thing interacting is the player character, get its controller
	if (Caller)
	{
		PC = Cast<APlayerController>(Caller->GetInstigatorController());
	}

	// Fallback: just grab player 0
	if (!PC)
	{
		PC = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr;
	}

	if (!PC)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Red, TEXT("No PlayerController to show letter!"));
		}
		return;
	}

	// If we have a widget class assigned, create and add it
	if (LetterWidgetClass)
	{
		UUserWidget* LetterWidget = CreateWidget<UUserWidget>(PC, LetterWidgetClass);
		if (LetterWidget)
		{
			LetterWidget->AddToViewport(10); // Z-order

			// Pause the game when letter is being read
			PC->SetPause(true);
			PC->bShowMouseCursor = true;

			if (URY_LetterWidgetBase* Typed = Cast<URY_LetterWidgetBase>(LetterWidget))
			{
				Typed->SetupLetter(LetterTitle, LetterBody);
			}
		}
	}
	else
	{
		// Fallback: just print the letter on screen (quick debug)
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Cyan,
			                                 FString::Printf(TEXT("%s\n\n%s"),
			                                                 *LetterTitle.ToString(),
			                                                 *LetterBody.ToString()));
		}
	}

	if (bDestroyAfterRead)
	{
		Destroy();
	}
}
