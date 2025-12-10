#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HearingEnemyCharacter.generated.h"

class UPawnSensingComponent;
class UBehaviorTree;

/**
* AI Character that can hear sounds and react to them
*/
UCLASS()
class YORIAN_LITTLEPRESTON_API AHearingEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHearingEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Pawn sensing component using TObjectPtr
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI")
	TObjectPtr<UPawnSensingComponent> PawnSensingComponent;

	// Executed when noise is heard from a pawn
	UFUNCTION()
	void OnHearNoise(APawn* InstigatorPawn, const FVector& Location, float Volume);

	// Behavior tree
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;
};
