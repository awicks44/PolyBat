// Fill out your copyright notice in the Description page of Project Settings.


#include "Scroller.h"

#include "GothamTileManager.h"
#include "UtilityLib.h"

// Sets default values for this component's properties
UScroller::UScroller()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UScroller::BeginPlay()
{
	Super::BeginPlay();

	FTileManagerResult Result = UUtilityLib::GetTileManager(GetWorld());
	if (Result.bIsFound)
	{
		TileManager = Cast<AGothamTileManager>(Result.Actor);
	}
}


// Called every frame
void UScroller::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!TileManager)
		return;

	// get actor forward vector, multiply forward direction by speed (which will be a negative value)
	// multiply by deltaTime to make it framerate independent
	DestinationOffset = GetOwner()->GetActorForwardVector() * TileManager->GetTileSpeed() * DeltaTime;

	GetOwner()->AddActorLocalOffset(DestinationOffset);
}

