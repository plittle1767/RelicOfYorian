#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStatsWidget.generated.h"

class UProgressBar;
class UTextBlock;
class ARY_Character;

UCLASS()
class YORIAN_LITTLEPRESTON_API UPlayerStatsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* StaminaBar;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* HealthText;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* StaminaText;

private:
	UPROPERTY()
	ARY_Character* OwnerPlayer;

	UFUNCTION()
	void OnHealthChanged(float Health, float MaxHealth, float HealthPercentage);

	UFUNCTION()
	void OnStaminaChanged(float Stamina, float MaxStamina, float StaminaPercentage);
};
