// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageField.h"
#include "HealthComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ADamageField::ADamageField()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	DamageBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DamageBox"));
	DamageBox->SetupAttachment(Root);
	DamageBox->SetBoxExtent(FVector(100.0f, 100.0f, 100.0f));
	DamageBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DamageBox->SetCollisionResponseToAllChannels(ECR_Overlap);

	DamageMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DamageMesh"));
	DamageMesh->SetupAttachment(Root);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("CubeMesh"));

	if (CubeMesh.Succeeded())
	{
		DamageMesh->SetStaticMesh(CubeMesh.Object);
	}

	DamageMesh->SetRelativeScale3D(FVector(2.0f, 2.0f, 2.0f));
	DamageMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ADamageField::BeginPlay()
{
	Super::BeginPlay();

	DamageBox->OnComponentBeginOverlap.AddDynamic(this, &ADamageField::OnOverlapBegin);
	DamageBox->OnComponentEndOverlap.AddDynamic(this, &ADamageField::OnOverlapEnd);
}

void ADamageField::ApplyDamage()
{
	if (OverlappingActor)
	{
		if (UHealthComponent* HealthComponent = OverlappingActor->FindComponentByClass<UHealthComponent>())
		{
			HealthComponent->TakeDamage(DamageAmount);
		}
	}
}

void ADamageField::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
								   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
								   const FHitResult& SweepResult)
{
	if (UHealthComponent* HealthComponent = OtherActor->FindComponentByClass<UHealthComponent>())
	{
		OverlappingActor = OtherActor;
		ApplyDamage();
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ADamageField::ApplyDamage, DamageInterval, true);
	}
}

void ADamageField::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
								 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == OverlappingActor)
	{
		GetWorldTimerManager().ClearTimer(TimerHandle);
		OverlappingActor = nullptr;
	}
}

