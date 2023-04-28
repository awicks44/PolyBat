// Fill out your copyright notice in the Description page of Project Settings.


#include "Bat.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ABat::ABat()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 90.f);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	GetCharacterMovement()->JumpZVelocity = 500.f;

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void ABat::BeginPlay()
{
	Super::BeginPlay();

	if (const APlayerController *PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem * Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(MappingContext, 0);
		}
	}
	CurrentPath = EPath::Middle;

	StartLocation = GetActorLocation();

	SetReadyToRun(true);
}

// Called every frame
void ABat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FVector CurrentLocation = GetActorLocation();
	const FVector Velocity = bSwitchingLanes ? MovementVelocity : FVector(MovementVelocity.X, 0, MovementVelocity.Z);
	CurrentLocation = CurrentLocation + (Velocity * DeltaTime);
	SetActorLocation(CurrentLocation);
	UE_LOG(LogTemp, Warning, TEXT("Batman (C++) | Location: %s"),*CurrentLocation.ToString());
	
	if (bSwitchingLanes)
	{
		// re-calculate path positions
		RightLocation = FVector(CurrentLocation.X, MoveDistance, CurrentLocation.Z);
		MiddleLocation = FVector(CurrentLocation.X, 0,  CurrentLocation.Z);
		LeftLocation = FVector(CurrentLocation.X, -MoveDistance,  CurrentLocation.Z);

		// calculate distance between start and current location
		const FVector Start = FVector(0, StartLocation.Y, 0);
		const FVector Curr = FVector(0, CurrentLocation.Y, 0);
		
		const float DistanceMoved = FVector::Dist(Start, Curr);

		UE_LOG(LogTemp, Error, TEXT("Batman (C++) |Distanced Moved %f "), DistanceMoved);

		if (DistanceMoved > MoveDistance)
		{
			bSwitchingLanes = false;
			
			const FVector MoveDirection = MovementVelocity.GetSafeNormal();
			UE_LOG(LogTemp, Error, TEXT("Batman (C++) | DistanceMoved %f | Movement Velocity %s | MoveDirection %s "), DistanceMoved, *MovementVelocity.ToString(), *MoveDirection.ToString());
			StartLocation = StartLocation + MoveDirection * MoveDistance;

			if (FVector::PointsAreNear(StartLocation, MiddleLocation, 25.f))
			{
				UE_LOG(LogTemp, Error, TEXT("Batman (C++) | Middle Location %s "), *GetActorLocation().ToString());
				StartLocation = MiddleLocation;
			}
			else if (FVector::PointsAreNear(StartLocation, RightLocation, 25.f))
			{
				UE_LOG(LogTemp, Error, TEXT("Batman (C++) | Right Location %s "), *GetActorLocation().ToString());
				StartLocation = RightLocation;
			}
			else if (FVector::PointsAreNear(StartLocation, LeftLocation, 25.f))
			{
				UE_LOG(LogTemp, Error, TEXT("Batman (C++) | Left Location %s "), *GetActorLocation().ToString());
				StartLocation = LeftLocation;
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Batman (C++) | Not Near | Curr: %s | Start:%s  "), *CurrentLocation.ToString(), *StartLocation.ToString());
			}

			SetActorLocation(StartLocation);
		}
	}
}

// Called to bind functionality to input
void ABat::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent *EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &ABat::MoveRight);
		EnhancedInputComponent->BindAction(MoveLeftAction, ETriggerEvent::Triggered, this, &ABat::MoveLeft);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ABat::MakeJump);
		EnhancedInputComponent->BindAction(KickAction, ETriggerEvent::Triggered, this, &ABat::Kick);
		EnhancedInputComponent->BindAction(PunchAction, ETriggerEvent::Triggered, this, &ABat::Punch);
	}
}

void ABat::MoveRight(const FInputActionValue& Value)
{
	if (const bool IsPressed = Value.Get<bool>() && CurrentPath != EPath::Right)
	{
		UE_LOG(LogTemp, Warning, TEXT("Batman (C++) | Move Right"));

		// need to recalculate the start position every time they move right or left
		StartLocation = GetActorLocation();
		
		bSwitchingLanes = true;
		// ensure that movement velocity is always going right when they press right (make it positive)
		MovementVelocity = MovementVelocity.GetAbs();
		PreviousPath = CurrentPath;

		UE_LOG(LogTemp, Error, TEXT("Batman (C++) | Previous Path: %s | Current Path: %s "), *UEnum::GetValueAsString(PreviousPath), *UEnum::GetValueAsString(CurrentPath));
		
		if (CurrentPath == EPath::Middle)
		{
			CurrentPath = EPath::Right;
			UE_LOG(LogTemp, Warning, TEXT("Batman (C++) | Path -> Right"));
		}
		else if (CurrentPath == EPath::Left)
		{
			CurrentPath = EPath::Middle;
			UE_LOG(LogTemp, Warning, TEXT("Batman (C++) | Path -> Middle"));
		}

		UE_LOG(LogTemp, Error, TEXT("Batman (C++) | Previous Path: %s | Current Path: %s "), *UEnum::GetValueAsString(PreviousPath), *UEnum::GetValueAsString(CurrentPath));
	}
}

void ABat::MoveLeft(const FInputActionValue& Value)
{
	if (const bool IsPressed = Value.Get<bool>() && CurrentPath != EPath::Left)
	{
		// need to recalculate the start position every time they move right or left
		StartLocation = GetActorLocation();
		
		bSwitchingLanes = true;

		// ensure that movement velocity is always going left when they press left (make it negative)
		MovementVelocity = FVector(MovementVelocity.X, -(MovementVelocity.GetAbs().Y), MovementVelocity.GetAbs().Z);
		
		PreviousPath = CurrentPath;
		
		if (CurrentPath == EPath::Middle)
		{
			CurrentPath = EPath::Left;
		}
		else if (CurrentPath == EPath::Right)
		{
			CurrentPath = EPath::Middle;
		}
	}
}

void ABat::Kick_Implementation(const FInputActionValue& Value)
{
	if (const bool IsPressed = Value.Get<bool>())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bat (C++) | Kick"));

		if (KickMontage.Num() > 0)
		{
			PlayAnimMontage(KickMontage[0]);
		}
	}
}

void ABat::Punch_Implementation(const FInputActionValue& Value)
{
	if (const bool IsPressed = Value.Get<bool>())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bat (C++) | Punch"));

		if (PunchMontage.Num() > 0)
		{
			PlayAnimMontage(PunchMontage[0]);
		}
	}
}

void ABat::MakeJump_Implementation(const FInputActionValue& Value)
{
	if (const bool IsPressed = Value.Get<bool>())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bat (C++) | Make Jump"));
		Jump();
	}
}



