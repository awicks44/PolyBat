// Fill out your copyright notice in the Description page of Project Settings.


#include "GothamTileManager.h"

#include "PolyBat/Enviroment/GothamTile.h"

// Sets default values
AGothamTileManager::AGothamTileManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGothamTileManager::BeginPlay()
{
	Super::BeginPlay();

	FVector Origin;
	FVector BoxExtent;
	
	AActor * FirstGothamTileActor = GetWorld()->SpawnActor(GothamTileClass);
	// get the actor dimensions
	FirstGothamTileActor->GetActorBounds(true, Origin, BoxExtent);
	// the box extent is only half the size of the x dimensions so multiple it to get the full x amount
	const float TrailingTileStartX = -BoxExtent.X * 2;
	// set the trailing actor x position using the variable above
	AActor * TrailingGothamActor = GetWorld()->SpawnActor<AActor>(GothamTileClass, FVector(TrailingTileStartX, 0, 0), FRotator());
	// add to the array
	AGothamTile * GothamTileFront = Cast<AGothamTile>(FirstGothamTileActor);
	GothamTileFront->SetIndex(0);

	AGothamTile * GothamTileTrailing = Cast<AGothamTile>(TrailingGothamActor);
	GothamTileTrailing->SetIndex(1);

	GothamTileFront->SetSiblingIndex(GothamTileTrailing->GetIndex());
	GothamTileTrailing->SetSiblingIndex(GothamTileFront->GetIndex());
	GothamTiles.Add(GothamTileFront);
	GothamTiles.Add(GothamTileTrailing);
}

// Called every frame
void AGothamTileManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	int SiblingIdx = -1;

	for (const auto GothamTile : GothamTiles)
	{
		if (GothamTile->ExitPointReached())
		{
			SiblingIdx = GothamTile->GetSiblingIndex();
			UE_LOG(LogTemp, Error, TEXT("GothamTileManager (C++) | Gotham Tile Change Position | Sibling: %d"), SiblingIdx);
			GothamTile->ResetTriggerState();
			break;
		}
	}

	if (SiblingIdx != -1)
	{
		auto GothamTile = GothamTiles.FindByPredicate([&SiblingIdx](const AGothamTile *Tile)
		{
			return Tile->GetIndex() == SiblingIdx;
		});

		if (GothamTile)
		{
			UE_LOG(LogTemp, Error, TEXT("GothamTileManager (C++) | Found Sibling Gotham Tile"))
			(*GothamTile)->ChangePosition();
			SiblingIdx = -1;
		}
	}

}

