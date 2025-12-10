#include "RY_Projectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/Engine.h"
#include "GeometryCollection/GeometryCollectionActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

// Sets default values
ARY_Projectile::ARY_Projectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create sphere collision component
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	RootComponent = SphereCollision;
	SphereCollision->SetSphereRadius(15.0f);
	SphereCollision->SetCollisionProfileName(TEXT("BlockAll"));
	SphereCollision->SetNotifyRigidBodyCollision(true); // Enable hit events

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	SetRootComponent(ProjectileMesh);

	// We want hits + overlaps
	ProjectileMesh->SetNotifyRigidBodyCollision(true);
	ProjectileMesh->SetGenerateOverlapEvents(true);
	ProjectileMesh->SetSimulatePhysics(false); // projectile movement handles motion

	// Use a custom or "Projectile" collision preset in the editor
	// ProjectileMesh->SetCollisionProfileName(TEXT("Projectile"));

	ProjectileMesh->OnComponentHit.AddDynamic(this, &ARY_Projectile::OnHit);
	ProjectileMesh->OnComponentBeginOverlap.AddDynamic(this, &ARY_Projectile::OnBeginOverlap);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 1500.f;
	ProjectileMovement->MaxSpeed = 1500.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 1.f; // gives the throw a nice arc
}

// Called when the game starts or when spawned
void ARY_Projectile::BeginPlay()
{
	Super::BeginPlay();

	SphereCollision->OnComponentHit.AddDynamic(this, &ARY_Projectile::OnHit);
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ARY_Projectile::OnBeginOverlap);
}

// Called every frame
void ARY_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARY_Projectile::InitVelocity(const FVector& ShootDirection)
{
	if (ProjectileMovement)
	{
		ProjectileMovement->Velocity = ShootDirection * ProjectileMovement->InitialSpeed;
	}
}

void ARY_Projectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor || OtherActor == this) { return; }

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow,
	                                 FString::Printf(TEXT("Hit: %s"), *OtherActor->GetName()));

	// Apply impulse to physics objects (boxes, barrels, etc.)
	if (OtherComp && OtherComp->IsSimulatingPhysics())
	{
		FVector ImpulseDirection = GetVelocity().GetSafeNormal();
		OtherComp->AddImpulseAtLocation(ImpulseDirection * ImpulseStrength, Hit.Location);
	}

	// Handle Chaos Destruction (statues, breakable objects)
	if (AGeometryCollectionActor* GeoActor = Cast<AGeometryCollectionActor>(OtherActor))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green,
		                                 TEXT("CAST SUCCESS - Is GeometryCollectionActor!"));

		if (UGeometryCollectionComponent* GeoComp = GeoActor->GetGeometryCollectionComponent())
		{
			// Apply breaking impulse
			FVector ImpulseDirection = GetVelocity().GetSafeNormal();
			float BreakingForce = ImpulseStrength * 2.0f; // Stronger for breaking

			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan,
			                                 FString::Printf(TEXT("Breaking Force: %f"), BreakingForce));


			GeoComp->ApplyBreakingLinearVelocity(-1, ImpulseDirection * BreakingForce);
		}
	}

	// For demo: destroy on block so it's obvious
	Destroy();
}

void ARY_Projectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                         const FHitResult& SweepResult)
{
	if (OtherActor == nullptr || OtherActor == this || OtherActor == GetOwner()) return;

	UE_LOG(LogTemp, Warning, TEXT("Projectile overlapped with: %s"), *OtherActor->GetName());

	// Example: you could do damage, break crate, etc., then destroy the rock
	// Destroy();
}
