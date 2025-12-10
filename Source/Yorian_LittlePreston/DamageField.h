#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageField.generated.h"

class UBoxComponent;

UCLASS()
class YORIAN_LITTLEPRESTON_API ADamageField : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADamageField();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UBoxComponent* DamageBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UStaticMeshComponent* DamageMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
	float DamageAmount = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
	float DamageInterval = 1.f;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                  int32 OtherBodyIndex);

private:
	FTimerHandle TimerHandle;

	UPROPERTY()
	AActor* OverlappingActor;

	void ApplyDamage();
};
