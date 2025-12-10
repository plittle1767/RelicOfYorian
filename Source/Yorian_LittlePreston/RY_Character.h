// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "RY_Character.generated.h"

// Custom classes
class URY_InteractionComponent;
class URY_InventoryComponent;
class URY_Projectile;
class UHealthComponent;
class UStaminaComponent;

// Unreal classes
class UInputMappingContext;
class UInputAction;
class UInputComponent;
class USpringArmComponent;
class UCameraComponent;
class UPawnNoiseEmitterComponent;

UCLASS()
class YORIAN_LITTLEPRESTON_API ARY_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARY_Character();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Helper so other actors (like collectables) can call this from C++
	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool AddItemToInventory(FName ItemID, TSubclassOf<AActor> SourceClass, int32 Amount = 1);

	// Function to play and report sound
	UFUNCTION(BlueprintCallable, Category="AI")
	void ReportNoise(USoundBase* SoundToPlay, float Volume);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interact")
	TObjectPtr<URY_InteractionComponent> InteractionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory")
	TObjectPtr<URY_InventoryComponent> InventoryComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile")
	TSubclassOf<class ARY_Projectile> ProjectileClass;

	/** Health and Stamina Components */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Attributes")
	UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Attributes")
	UStaminaComponent* StaminaComponent;

	/** The Input Action for moving. You will assign IA_Move to this
	in your Character Blueprint. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	TObjectPtr<UInputAction> MoveAction;

	/** The Input Action for look. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	TObjectPtr<UInputAction> LookAction;

	/** The Input Action for jumping. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	TObjectPtr<UInputAction> InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	TObjectPtr<UInputAction> RunAction;

	/** The Input Action for throwing */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	TObjectPtr<UInputAction> ThrowAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement|Speed")
	float WalkSpeed = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement|Speed")
	float RunSpeed = 900.f;

	/** Stamina Properties */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stamina")
	float SprintStaminaDrainPerSecond = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stamina")
	float MinStaminaToSprint = 5.f;

	bool bIsSprinting = false;

	bool IsMovingOnGround() const;

	/** UI Widget */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI")
	TSubclassOf<class UPlayerStatsWidget> StatsWidgetClass;

	UPROPERTY()
	UPlayerStatsWidget* StatsWidget;

	/** Function to handle the inputs */
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void SingleJump(const FInputActionValue& Value);
	void Interact();
	// void Run(const FInputActionValue& Value);
	void StartRun();
	void StopRun();
	void ThrowProjectile();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Add this component using TObjectPtr
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="AI")
	TObjectPtr<UPawnNoiseEmitterComponent> PawnNoiseEmitterComponent;

public:
	/** Attributes Accessors */
	UFUNCTION(BlueprintPure, Category="Attributes")
	UHealthComponent* GetHealthComponent() const { return HealthComponent; }

	UFUNCTION(BlueprintPure, Category="Attributes")
	UStaminaComponent* GetStaminaComponent() const { return StaminaComponent; }
};
