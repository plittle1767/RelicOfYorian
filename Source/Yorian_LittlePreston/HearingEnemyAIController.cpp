#include "HearingEnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "HearingEnemyCharacter.h"

AHearingEnemyAIController::AHearingEnemyAIController()
{
	// Create components
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
}

void AHearingEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (AHearingEnemyCharacter* HearingCharacter = Cast<AHearingEnemyCharacter>(InPawn))
	{
		if (HearingCharacter->BehaviorTree)
		{
			if (HearingCharacter->BehaviorTree->BlackboardAsset)
			{
				BlackboardComponent->InitializeBlackboard(*HearingCharacter->BehaviorTree->BlackboardAsset);
			}

			BehaviorTreeComponent->StartTree(*HearingCharacter->BehaviorTree);
		}
	}
}

void AHearingEnemyAIController::SetSensedTarget(APawn* NewTarget)
{
	if (BlackboardComponent && NewTarget)
	{
		BlackboardComponent->SetValueAsObject(SensedPawnKeyName, NewTarget);
	}
}
