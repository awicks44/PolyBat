// Fill out your copyright notice in the Description page of Project Settings.


#include "Batman.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

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
	
	CurrentPath = EPath::Middle;
	
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

	StartLocation = GetActorLocation();
}

void ABatman::MoveRight(const FInputActionValue& Value)
{
	if (const bool IsPressed = Value.Get<bool>() && CurrentPath != EPath::Right)
	{
		UE_LOG(LogTemp, Warning, TEXT("Batman (C++) | Move Right"));
		
		bIsMoving = true;
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

void ABatman::MoveLeft(const FInputActionValue& Value)
{
	if (const bool IsPressed = Value.Get<bool>() && CurrentPath != EPath::Left)
	{
		//MovementVelocity *= -1;
		bIsMoving = true;
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

// Called every frame
void ABatman::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bIsMoving)
	{
		FVector CurrentLocation = GetActorLocation();
		CurrentLocation = CurrentLocation + (MovementVelocity * DeltaTime);
		SetActorLocation(CurrentLocation);

		UE_LOG(LogTemp, Warning, TEXT("Batman (C++) | Movement Velocity %s Location %s "),*MovementVelocity.ToString(), *GetActorLocation().ToString());

		RightLocation = FVector(CurrentLocation.X, MovementAmount, 90);
		MiddleLocation = FVector(CurrentLocation.X, 0, 90);
		LeftLocation = FVector(CurrentLocation.X, -MovementAmount, 90);
		
		const float DistanceMoved = FVector::Dist(StartLocation, CurrentLocation);

		if (DistanceMoved > MoveDistance)
		{
			const FVector MoveDirection = MovementVelocity.GetSafeNormal();
			UE_LOG(LogTemp, Error, TEXT("Batman (C++) | DistanceMoved %f | Movement Velocity %s | MoveDirection %s "), DistanceMoved, *MovementVelocity.ToString(), *MoveDirection.ToString());
			StartLocation = StartLocation + MoveDirection * MoveDistance;
			SetActorLocation(StartLocation);
			
			bIsMoving = false;

			if (FVector::PointsAreNear(StartLocation, MiddleLocation, 25.f))
			{
				UE_LOG(LogTemp, Warning, TEXT("Batman (C++) | Middle Location %s "), *GetActorLocation().ToString());
				SetActorLocation(MiddleLocation);
				StartLocation = MiddleLocation;
			}
			else if (FVector::PointsAreNear(StartLocation, RightLocation, 25.f))
			{
				UE_LOG(LogTemp, Warning, TEXT("Batman (C++) | Right Location %s "), *GetActorLocation().ToString());
				SetActorLocation(RightLocation);
				StartLocation = RightLocation;
			}
			else if (FVector::PointsAreNear(StartLocation, LeftLocation, 25.f))
			{
				UE_LOG(LogTemp, Warning, TEXT("Batman (C++) | Left Location %s "), *GetActorLocation().ToString());
				SetActorLocation(LeftLocation);
				StartLocation = LeftLocation;
			}
		}
	}
}

// Called to bind functionality to input
void ABatman::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent *EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &ABatman::MoveRight);
		EnhancedInputComponent->BindAction(MoveLeftAction, ETriggerEvent::Triggered, this, &ABatman::MoveLeft);
	}
}

