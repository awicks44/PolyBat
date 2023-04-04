// Fill out your copyright notice in the Description page of Project Settings.


#include "Bat.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

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
}

// Called every frame
void ABat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FVector CurrentLocation = GetActorLocation();
	FVector Velocity = bSwitchingLanes ? MovementVelocity : FVector(MovementVelocity.X, 0, MovementVelocity.Z);

	if (!bSwitchingLanes)
	{
		CurrentLocation = CurrentLocation + (Velocity * DeltaTime);
		SetActorLocation(CurrentLocation);
		UE_LOG(LogTemp, Warning, TEXT("Batman (C++) | Location:%s"),*CurrentLocation.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Batman (C++) | Location Before Applying Velocity %s"),*CurrentLocation.ToString());
		CurrentLocation = CurrentLocation + (MovementVelocity * DeltaTime);
		UE_LOG(LogTemp, Warning, TEXT("Batman (C++) | Location Before After Velocity %s"),*CurrentLocation.ToString());
		SetActorLocation(CurrentLocation);

		UE_LOG(LogTemp, Warning, TEXT("Batman (C++) | Movement Velocity %s "),*MovementVelocity.ToString());

		// re-calculate path positions
		RightLocation = FVector(CurrentLocation.X, MoveDistance, CurrentLocation.Z);
		MiddleLocation = FVector(CurrentLocation.X, 0,  CurrentLocation.Z);
		LeftLocation = FVector(CurrentLocation.X, -MoveDistance,  CurrentLocation.Z);

		// calculate distance between start and current location
		const float DistanceMoved = FVector::Dist(StartLocation, CurrentLocation);

		if (DistanceMoved > MoveDistance)
		{
			const FVector MoveDirection = MovementVelocity.GetSafeNormal();
			UE_LOG(LogTemp, Error, TEXT("Batman (C++) | DistanceMoved %f | Movement Velocity %s | MoveDirection %s "), DistanceMoved, *MovementVelocity.ToString(), *MoveDirection.ToString());
			StartLocation = StartLocation + MoveDirection * MoveDistance;
			SetActorLocation(StartLocation);
			
			bSwitchingLanes = false;

			if (FVector::PointsAreNear(StartLocation, MiddleLocation, 25.f))
			{
				UE_LOG(LogTemp, Error, TEXT("Batman (C++) | Middle Location %s "), *GetActorLocation().ToString());
				SetActorLocation(MiddleLocation);
				StartLocation = MiddleLocation;
			}
			else if (FVector::PointsAreNear(StartLocation, RightLocation, 25.f))
			{
				UE_LOG(LogTemp, Error, TEXT("Batman (C++) | Right Location %s "), *GetActorLocation().ToString());
				SetActorLocation(RightLocation);
				StartLocation = RightLocation;
			}
			else if (FVector::PointsAreNear(StartLocation, LeftLocation, 25.f))
			{
				UE_LOG(LogTemp, Error, TEXT("Batman (C++) | Left Location %s "), *GetActorLocation().ToString());
				SetActorLocation(LeftLocation);
				StartLocation = LeftLocation;
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Batman (C++) | Not Near | Curr: %s | Start:%s  "), *GetActorLocation().ToString(), *StartLocation.ToString());
			}
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
	}
}

void ABat::MoveRight(const FInputActionValue& Value)
{
	if (const bool IsPressed = Value.Get<bool>() && CurrentPath != EPath::Right)
	{
		UE_LOG(LogTemp, Warning, TEXT("Batman (C++) | Move Right"));
		
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
		bSwitchingLanes = true;

		// ensure that movement velocity is always going left when they press left (make it negative)
		MovementVelocity = -MovementVelocity.GetAbs();
		
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


