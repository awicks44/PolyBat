// Fill out your copyright notice in the Description page of Project Settings.


#include "GothamTile.h"

#include "Components/BoxComponent.h"

// Sets default values
AGothamTile::AGothamTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(GetRootComponent());

	Trigger = CreateDefaultSubobject<UBoxComponent>("TriggerArea");
	Trigger->SetupAttachment(Mesh);

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AGothamTile::ExitTriggerOverlapped);

}

// Called when the game starts or when spawned
void AGothamTile::BeginPlay()
{
	Super::BeginPlay();

	MovementState = EMovementState::SetInLocation;
	GothamTriggerState = EGothamTriggerState::Trigger_Waiting;
}

// Called every frame
void AGothamTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGothamTile::ChangePosition()
{
	MovementState = EMovementState::SetInLocation;

	const FVector CurrentPosition = GetActorLocation();
	// get the mesh size going in x direction
	FVector Origin;
	FVector BoxExtent;
	GetActorBounds(true, Origin, BoxExtent);

	const float XSize = BoxExtent.X * 2;
	const float NewX = XSize * 2 + CurrentPosition.X;

	const FVector NewPosition = FVector(NewX, CurrentPosition.Y, CurrentPosition.Z);
	SetActorLocation(NewPosition);
	
}

void AGothamTile::ExitTriggerOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->ActorHasTag("Batman") && GothamTriggerState != EGothamTriggerState::Trigger_Hit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hi Batman"));
		GothamTriggerState = EGothamTriggerState::Trigger_Hit;
		
	}
}


