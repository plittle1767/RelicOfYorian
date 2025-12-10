#include "HearingEnemyCharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "HearingEnemyAIController.h"

// Sets default values
AHearingEnemyCharacter::AHearingEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize components
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));

	// Configure pawn sensing
	if (PawnSensingComponent)
	{
		PawnSensingComponent->HearingThreshold = 1400.0f;
		PawnSensingComponent->LOSHearingThreshold = 2800.0f;
		PawnSensingComponent->SightRadius = 5000.0f;
		PawnSensingComponent->SensingInterval = 0.5f;
		PawnSensingComponent->HearingMaxSoundAge = 1.0f;
		PawnSensingComponent->bEnableSensingUpdates = true;
		PawnSensingComponent->bOnlySensePlayers = true;
		PawnSensingComponent->bSeePawns = true;
		PawnSensingComponent->bHearNoises = true; // Crucial for hearing!
		PawnSensingComponent->SetPeripheralVisionAngle(90.0f);
	}
}

// Called when the game starts or when spawned
void AHearingEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Bind the hearing delegate
	if (PawnSensingComponent)
	{
		PawnSensingComponent->OnHearNoise.AddDynamic(this, &AHearingEnemyCharacter::OnHearNoise);
	}
}

// Called every frame
void AHearingEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AHearingEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AHearingEnemyCharacter::OnHearNoise(APawn* InstigatorPawn, const FVector& Location, float Volume)
{
	// Get the AI controller
	if (AHearingEnemyAIController* AIController = Cast<AHearingEnemyAIController>(GetController()))
	{
		// Make sure the AI doesn't sense its own sounds
		if (InstigatorPawn != this)
		{
			AIController->SetSensedTarget(InstigatorPawn);
		}
	}
}
