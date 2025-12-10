#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "HearingEnemyAIController.generated.h"

class UBlackboardComponent;
class UBehaviorTreeComponent;

/**
* AI Controller for hearing-based enemies
*/
UCLASS()
class YORIAN_LITTLEPRESTON_API AHearingEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AHearingEnemyAIController();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI")
	TObjectPtr<UBlackboardComponent> BlackboardComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI")
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;

	const FName SensedPawnKeyName = FName("SensedPawn");

	virtual void OnPossess(APawn* InPawn) override;

public:
	// Set the target the AI has sensed
	UFUNCTION(BlueprintCallable, Category="AI")
	void SetSensedTarget(APawn* NewTarget);
};
