#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth, GetPercentHealth());
}

void UHealthComponent::TakeDamage(float DamageAmount)
{
	if (isCharacterDead || DamageAmount <= 0) { return; }

	SetHealth(CurrentHealth - DamageAmount);
}

void UHealthComponent::Heal(float HealAmount)
{
	if (isCharacterDead || HealAmount <= 0) { return; }

	SetHealth(CurrentHealth + HealAmount);
}

void UHealthComponent::SetHealth(float NewHealth)
{
	CurrentHealth = FMath::Clamp(NewHealth, 0, MaxHealth);
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth, GetPercentHealth());

	// Do we need to broadcast death?
	if (CurrentHealth <= 0 && !isCharacterDead)
	{
		OnDeath.Broadcast();
		isCharacterDead = true;
	}
}
