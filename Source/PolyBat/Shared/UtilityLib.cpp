// Fill out your copyright notice in the Description page of Project Settings.


#include "UtilityLib.h"

#include "GothamTileManager.h"
#include "Kismet/GameplayStatics.h"

FTileManagerResult UUtilityLib::GetTileManager(UObject * WorldContextObject)
{
	const UWorld * World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	
	FTileManagerResult Result;
	Result.bIsFound = false;
	
	if (World)
	{
		TArray<AActor *> Actors;
		UGameplayStatics::GetAllActorsOfClass(World, AGothamTileManager::StaticClass(), Actors);
		Result.bIsFound =  Actors.Num() > 0;
		Result.Actor = Actors[0];
	}

	return Result;
}
