#include "StaminaComponent.h"

// Sets default values for this component's properties
UStaminaComponent::UStaminaComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentStamina = MaxStamina;
	OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina, GetStaminaPercent());
}

// Called every frame
void UStaminaComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	RegenerateStamina(DeltaTime);
}

bool UStaminaComponent::UseStamina(float StaminaAmount)
{
	if (StaminaAmount <= 0.f) { return true; }

	if (CurrentStamina >= StaminaAmount)
	{
		// Reset regeneration delay
		TimeSinceLastStaminaUse = 0.f;
		SetStamina(CurrentStamina - StaminaAmount);

		return true;
	}

	return false;
}

void UStaminaComponent::SetStamina(float NewStamina)
{
	CurrentStamina = FMath::Clamp(NewStamina, 0.f, MaxStamina);
	OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina, GetStaminaPercent());
}

void UStaminaComponent::RestoreStamina(float StaminaAmount)
{
	if (StaminaAmount <= 0.f) { return; }

	SetStamina(CurrentStamina + StaminaAmount);
}

void UStaminaComponent::RegenerateStamina(float DeltaTime)
{
	TimeSinceLastStaminaUse += DeltaTime;

	if (TimeSinceLastStaminaUse >= DelayBeforeRegeneration && CurrentStamina < MaxStamina)
	{
		SetStamina(CurrentStamina + (RegenerationRate * DeltaTime));
	}
}
