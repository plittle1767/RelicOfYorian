#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StaminaComponent.generated.h"

// Delegates for health change and death
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnStaminaChanged, float, Stamina, float, MaxStamina, float, Perfect);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class YORIAN_LITTLEPRESTON_API UStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStaminaComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float MaxStamina = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stamina")
	float CurrentStamina = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float RegenerationRate = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float DelayBeforeRegeneration = 5.f;

	UPROPERTY(BlueprintAssignable, Category = "Stamina")
	FOnStaminaChanged OnStaminaChanged;

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	bool UseStamina(float StaminaAmount);

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void RestoreStamina(float StaminaAmount);

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void SetStamina(float NewStamina);

	UFUNCTION(BlueprintPure, Category = "Stamina")
	float GetStamina() const { return CurrentStamina; }

	UFUNCTION(BlueprintPure, Category = "Stamina")
	float GetMaxStamina() const { return MaxStamina; }

	UFUNCTION(BlueprintPure, Category = "Stamina")
	float GetStaminaPercent() const { return CurrentStamina / MaxStamina; }

	UFUNCTION(BlueprintPure, Category = "Stamina")
	bool HasStamina(float Amount) const { return CurrentStamina >= Amount; }

private:
	float TimeSinceLastStaminaUse = 0.f;

	void RegenerateStamina(float DeltaTime);
};
