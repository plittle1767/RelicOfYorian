#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyAIController.generated.h"

// Forward declarations to avoid circular dependencies
class UBlackboardComponent;
class UBehaviorTreeComponent;
class UAISenseConfig_Sight;

/**
 * AI Controller for enemies using Environment Query System (EQS)
 */
UCLASS()
class YORIAN_LITTLEPRESTON_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController();

protected:
	// Components using TObjectPtr
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UBlackboardComponent> BlackboardComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;

	// Perception configuration
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UAISenseConfig_Sight> SightConfig;

	// Blackboard key names
	const FName EnemyKeyName = FName("EnemyKey");

	// Perception callback
	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);
	virtual void OnPossess(APawn* InPawn) override;

public:
	// Get the actor the AI is currently tracking
	UFUNCTION(BlueprintCallable, Category = "AI")
	AActor* GetSeeingPawn() const;
};
