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

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AGothamTile::AdjustPosition);

}

// Called when the game starts or when spawned
void AGothamTile::BeginPlay()
{
	Super::BeginPlay();

	MovementState = EMovementState::SetInLocation;
}

// Called every frame
void AGothamTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGothamTile::AdjustPosition(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->ActorHasTag("Batman") && MovementState != EMovementState::ChangingLocation)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hi Batman"));
		MovementState = EMovementState::ChangingLocation;

		// get current position
		const FVector CurrentPosition = GetActorLocation();
		// get the mesh size going in x direction
		FVector Origin;
		FVector BoxExtent;
		GetActorBounds(true, Origin, BoxExtent);
		UE_LOG(LogTemp, Warning, TEXT("Gotham Tile (C++) | X Units: %f"), BoxExtent.X);
		// add 3/4 of the size and add it to the current position (to make it the new position)
		const float XOffset = BoxExtent.X + (BoxExtent.X * .75) ;
		const FVector NewPosition = FVector(CurrentPosition.X + XOffset, CurrentPosition.Y, CurrentPosition.Z);
		UE_LOG(LogTemp, Error, TEXT("Gotham Tile (C++) | New Position: %s"), *NewPosition.ToString());

		SetActorLocation(NewPosition);

		MovementState = EMovementState::SetInLocation;
	}
}


