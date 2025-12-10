#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "FindEnemyQueryContext.generated.h"

/**
 * EQS Context that provides the location of the enemy (player) that the AI is tracking
 */
UCLASS()
class YORIAN_LITTLEPRESTON_API UFindEnemyQueryContext : public UEnvQueryContext
{
	GENERATED_BODY()

public:
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
};
