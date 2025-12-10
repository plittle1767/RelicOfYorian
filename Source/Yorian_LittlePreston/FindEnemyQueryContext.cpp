#include "FindEnemyQueryContext.h"

#include "EnemyAIController.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"

void UFindEnemyQueryContext::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	// Get the owner of this query (should be the AI controller)
	AActor* QueryOwner = Cast<AActor>(QueryInstance.Owner.Get());

	if (!QueryOwner)
	{
		return;
	}

	// Cast to our AI controller
	AEnemyAIController* AIController =
		Cast<AEnemyAIController>(QueryOwner->GetInstigatorController());

	if (AIController)
	{
		// Get the actor the AI is currently seeing
		AActor* SeeingPawn = AIController->GetSeeingPawn();

		if (SeeingPawn)
		{
			// Provide this actor as the context for the query
			UEnvQueryItemType_Actor::SetContextHelper(ContextData, SeeingPawn);
		}
	}
}
