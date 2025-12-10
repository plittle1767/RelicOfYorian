#include "PlayerStatsWidget.h"
#include "RY_Character.h"
#include "HealthComponent.h"
#include "StaminaComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPlayerStatsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (HealthBar)
	{
		HealthBar->SetFillColorAndOpacity(FLinearColor::Red);
	}

	if (StaminaBar)
	{
		StaminaBar->SetFillColorAndOpacity(FLinearColor::Blue);
	}

	OwnerPlayer = Cast<ARY_Character>(GetOwningPlayerPawn());

	if (OwnerPlayer)
	{
		if (UHealthComponent* HealthComponent = OwnerPlayer->GetHealthComponent())
		{
			HealthComponent->OnHealthChanged.AddDynamic(this, &UPlayerStatsWidget::OnHealthChanged);
			OnHealthChanged(HealthComponent->GetHealth(), HealthComponent->GetMaxHealth(),
			                HealthComponent->GetPercentHealth());
		}

		if (UStaminaComponent* StaminaComponent = OwnerPlayer->GetStaminaComponent())
		{
			StaminaComponent->OnStaminaChanged.AddDynamic(this, &UPlayerStatsWidget::OnStaminaChanged);
			OnStaminaChanged(StaminaComponent->GetStamina(), StaminaComponent->GetMaxStamina(),
			                 StaminaComponent->GetStaminaPercent());
		}
	}
}

void UPlayerStatsWidget::NativeDestruct()
{
	// Destructor - unbind from component event
	if (OwnerPlayer)
	{
		if (UHealthComponent* HealthComponent = OwnerPlayer->GetHealthComponent())
		{
			HealthComponent->OnHealthChanged.AddDynamic(this, &UPlayerStatsWidget::OnHealthChanged);
		}

		if (UStaminaComponent* StaminaComponent = OwnerPlayer->GetStaminaComponent())
		{
			StaminaComponent->OnStaminaChanged.AddDynamic(this, &UPlayerStatsWidget::OnStaminaChanged);
		}
	}

	Super::NativeDestruct();
}

void UPlayerStatsWidget::OnHealthChanged(float Health, float MaxHealth, float HealthPercent)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(HealthPercent);
	}

	if (HealthText)
	{
		FString HealthString = FString::Printf(TEXT("%.2f/%.2f"), Health, MaxHealth);
		HealthText->SetText(FText::FromString(HealthString));
	}
}

void UPlayerStatsWidget::OnStaminaChanged(float Stamina, float MaxStamina, float StaminaPercent)
{
	if (StaminaBar)
	{
		StaminaBar->SetPercent(StaminaPercent);
	}

	if (StaminaText)
	{
		FString StaminaString = FString::Printf(TEXT("%.2f/%.2f"), Stamina, MaxStamina);
		StaminaText->SetText(FText::FromString(StaminaString));
	}
}
