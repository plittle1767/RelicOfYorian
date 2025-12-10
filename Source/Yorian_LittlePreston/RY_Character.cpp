// Fill out your copyright notice in the Description page of Project Settings.


#include "RY_Character.h"
#include "Camera/CameraComponent.h"
#include "Engine/Engine.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "HealthComponent.h"
#include "StaminaComponent.h"
#include "PlayerStatsWidget.h"
#include "Blueprint/UserWidget.h"
#include "RY_Projectile.h"
#include "RY_InteractionComponent.h"
#include "RY_InventoryComponent.h"
#include "RY_PlayerController.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARY_Character::ARY_Character()
{
	// Configure character to turn in the direction it's moving.
	// i.e. rotate to movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Movement values to change when necessary/to fine tune
	GetCharacterMovement()->JumpZVelocity = 300.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);

	// Let the spring arm follow the controller’s rotation (pitch/yaw)
	SpringArm->bUsePawnControlRotation = true;

	// Camera should NOT use pawn control rotation directly
	Camera->bUsePawnControlRotation = false;

	// Character itself should not use controller rotation for pitch/yaw/roll
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// This is fine for strafing-style movement
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// Interaction component
	InteractionComponent = CreateDefaultSubobject<URY_InteractionComponent>(TEXT("InteractionComponent"));

	// Inventory component
	InventoryComponent = CreateDefaultSubobject<URY_InventoryComponent>(TEXT("InventoryComponent"));

	// Health and Stamina Components
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	StaminaComponent = CreateDefaultSubobject<UStaminaComponent>(TEXT("StaminaComponent"));

	// Pawn noise emitter
	PawnNoiseEmitterComponent = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("PawnNoiseEmitterComponent"));
}

// Called when the game starts or when spawned
void ARY_Character::BeginPlay()
{
	Super::BeginPlay();

	// Ensure we start walking at WalkSpeed
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->MaxWalkSpeed = WalkSpeed;
	}

	ARY_PlayerController* PlayerController = Cast<ARY_PlayerController>(GetWorld()->GetFirstPlayerController());

	if (PlayerController)
	{
		StatsWidget = CreateWidget<UPlayerStatsWidget>(PlayerController, StatsWidgetClass);

		if (StatsWidget)
		{
			StatsWidget->AddToViewport();
			StatsWidget->SetOwningPlayer(PlayerController);
		}
	}
}

// Called every frame
void ARY_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	check(GEngine != nullptr);

	// Drain stamina only while the character is actually sprinting
	if (bIsSprinting && IsMovingOnGround())
	{
		if (UStaminaComponent* Stamina = GetStaminaComponent())
		{
			const float Cost = SprintStaminaDrainPerSecond * DeltaTime;
			const bool bPaid = Stamina->UseStamina(Cost);
			if (!bPaid)
			{
				StopRun();
			}
		}
	}
}

// Called to bind functionality to input
void ARY_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent =
		CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Bind the MoveAction to the Move function if the MoveAction is valid.
		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered,
			                                   this, &ARY_Character::Move);
		}
		if (LookAction)
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered,
			                                   this, &ARY_Character::Look);
		}
		if (JumpAction)
		{
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ARY_Character::SingleJump);
		}
		if (InteractAction)
		{
			EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ARY_Character::Interact);
		}
		if (RunAction)
		{
			EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &ARY_Character::StartRun);
			EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &ARY_Character::StopRun);
		}
		if (ThrowAction)
		{
			EnhancedInputComponent->BindAction(ThrowAction, ETriggerEvent::Started, this,
			                                   &ARY_Character::ThrowProjectile);
		}
	}
}

void ARY_Character::Move(const FInputActionValue& Value)
{
	// Get the 2D vector value from the input action.
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// Get the controller's rotation. We only want to move based on the Yaw.
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Get the forward and right direction vectors from the controller's yaw rotation.
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// Add movement input in the forward/backward and right/left directions.
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ARY_Character::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxis = Value.Get<FVector2D>();

	if (Controller)
	{
		// X = turn left/right (Yaw), Y = look up/down (Pitch)
		AddControllerYawInput(LookAxis.X);
		AddControllerPitchInput(LookAxis.Y);
	}
}

void ARY_Character::SingleJump(const FInputActionValue& Value)
{
	const bool bPressed = Value.Get<bool>();

	if (bPressed)
	{
		ACharacter::Jump();
	}
}

void ARY_Character::Interact()
{
	// GEngine->AddOnScreenDebugMessage(-1, 1.2f, FColor::Red, TEXT("Method is being called"));

	if (InteractionComponent)
	{
		InteractionComponent->LineTraceInteract();
	}
}

void ARY_Character::StartRun()
{
	// Must be on ground, moving, and have enough stamina to start
	if (!IsMovingOnGround()) return;

	if (UStaminaComponent* Stamina = GetStaminaComponent())
	{
		if (!Stamina->HasStamina(MinStaminaToSprint))
		{
			return; // Not enough to begin sprint
		}
	}

	bIsSprinting = true;

	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->MaxWalkSpeed = RunSpeed;
	}
}

void ARY_Character::StopRun()
{
	bIsSprinting = false;

	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->MaxWalkSpeed = WalkSpeed;
	}
}

bool ARY_Character::IsMovingOnGround() const
{
	const UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	if (!MoveComp) return false;

	if (!MoveComp->IsMovingOnGround()) return false;

	const FVector Vel = GetVelocity();
	const float HorizontalSpeedSq = FVector(Vel.X, Vel.Y, 0.f).SizeSquared();
	return HorizontalSpeedSq > 25.f; // small dead zone to avoid “standing still”
}

bool ARY_Character::AddItemToInventory(FName ItemID, TSubclassOf<AActor> SourceClass, int32 Amount)
{
	if (InventoryComponent)
	{
		return InventoryComponent->AddItem(ItemID, SourceClass, Amount);
	}
	return false;
}

void ARY_Character::ThrowProjectile()
{
	if (!ProjectileClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("ProjectileClass not set!"));
		return;
	}

	// Use camera direction so you throw where you look
	const FRotator ViewRot = GetControlRotation();
	const FVector ViewLoc = GetActorLocation() + FVector(0.f, 0.f, 60.f); // chest height-ish
	const FVector MuzzleLoc = ViewLoc + ViewRot.Vector() * 100.f;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	ARY_Projectile* Projectile = GetWorld()->SpawnActor<ARY_Projectile>(ProjectileClass, MuzzleLoc, ViewRot,
	                                                                    SpawnParams);

	if (Projectile)
	{
		Projectile->InitVelocity(ViewRot.Vector());
	}
}

void ARY_Character::ReportNoise(USoundBase* SoundToPlay, float Volume)
{
	if (SoundToPlay)
	{
		// 1. Play the sound for the player to hear (Visual/Audio feedback)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundToPlay, GetActorLocation(), Volume);

		// 2. Report noise to the AI System
		// Note: MakeNoise only works if called on the Server!
		MakeNoise(Volume, this, GetActorLocation());
	}
}
