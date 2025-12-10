// // Fill out your copyright notice in the Description page of Project Settings.
//
//
// #include "DestructibleActor.h"
// #include "GeometryCollection/GeometryCollectionComponent.h"
// #include "RY_Character.h"
//
// // Sets default values
// ADestructibleActor::ADestructibleActor()
// {
// 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
// 	PrimaryActorTick.bCanEverTick = false;
//
// 	GeometryComp = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryComp"));
// 	SetRootComponent(GeometryComp);
//
// 	GeometryComp->SetSimulatePhysics(true);
// 	GeometryComp->SetNotifyRigidBodyCollision(true); // for hit events
// }
//
// // Called when the game starts or when spawned
// void ADestructibleActor::BeginPlay()
// {
// 	Super::BeginPlay();
//
// 	GeometryComp->OnComponentHit.AddDynamic(this, &ADestructibleActor::OnGeomHit);
// }
//
// // Called every frame
// void ADestructibleActor::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
// }
//
// void ADestructibleActor::OnGeomHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
//                                    UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
// {
// 	if (ARY_Character* Player = Cast<ARY_Character>(OtherActor))
// 	{
// 		GeometryComp->ApplyDamageField(true, Hit.ImpactPoint, 200.f, 500.f);
// 	}
// }
