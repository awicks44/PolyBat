// Fill out your copyright notice in the Description page of Project Settings.


#include "Batman.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Shared/GothamTileManager.h"
#include "Shared/UtilityLib.h"

// Sets default values
ABatman::ABatman()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetCapsuleHalfHeight(90.f);
	Capsule->SetCapsuleRadius(25.f);
	SetRootComponent(Capsule);

	BatmanMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BatmanMesh"));
	BatmanMesh->SetupAttachment(GetRootComponent());

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void ABatman::Init()
{
	FTileManagerResult Result = UUtilityLib::GetTileManager(GetWorld());
	if (Result.bIsFound)
	{
		TileManager = Cast<AGothamTileManager>(Result.Actor);
		// get the middle lane
		CurrentLane = TileManager->GetNumberOfLanes() / 2 + 1;
		// get lane width
		LaneWidth = TileManager->GetLaneWidth();
	}
}

// Called when the game starts or when spawned
void ABatman::BeginPlay()
{
	Super::BeginPlay();

	if (const APlayerController *PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem * Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(MappingContext, 0);
		}
	}

	Init();
}

void ABatman::MoveRight_Implementation(const FInputActionValue& Value)
{
	if (const bool IsPressed = Value.Get<bool>())
	{
		// check to see if we are in the far-left lane
		if (CurrentLane < TileManager->GetNumberOfLanes() && !bIsMoving)
		{
			bIsMoving = true;

			// get where we minus the lane width to get the amount to move 
			TargetLocation = GetActorLocation().Y + LaneWidth;

			CurrentLane++;
		}
	}
}

void ABatman::MoveLeft_Implementation(const FInputActionValue& Value)
{
	if (const bool IsPressed = Value.Get<bool>())
	{
		// check to see if we are in the far-left lane
		if (CurrentLane > 1 && !bIsMoving)
		{
			bIsMoving = true;

			// get where we minus the lane width to get the amount to move 
			TargetLocation = GetActorLocation().Y - LaneWidth;

			CurrentLane--;
		}
	}
}

void ABatman::Punch_Implementation(const FInputActionValue& Value)
{
	if (const bool IsPressed = Value.Get<bool>())
	{
		
	}
}

void ABatman::Kick_Implementation(const FInputActionValue& Value)
{
	if (const bool IsPressed = Value.Get<bool>())
	{
		
	}
}

void ABatman::MakeJump_Implementation(const FInputActionValue& Value)
{
	if (const bool IsPressed = Value.Get<bool>())
	{
		
	}}

// Called every frame
void ABatman::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABatman::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent *EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &ABatman::MoveRight);
		EnhancedInputComponent->BindAction(MoveLeftAction, ETriggerEvent::Triggered, this, &ABatman::MoveLeft);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ABatman::MakeJump);
		EnhancedInputComponent->BindAction(KickAction, ETriggerEvent::Triggered, this, &ABatman::Kick);
		EnhancedInputComponent->BindAction(PunchAction, ETriggerEvent::Triggered, this, &ABatman::Punch);
	}
}

