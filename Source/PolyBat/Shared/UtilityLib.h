// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UtilityLib.generated.h"

USTRUCT(BlueprintType)
struct FTileManagerResult
{
	GENERATED_BODY()

	UPROPERTY()
	bool bIsFound;

	UPROPERTY()
	AActor * Actor; 
	
};

/**
 * 
 */
UCLASS()
class POLYBAT_API UUtilityLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category="PolyBat", meta=(WorldContext="WorldContextObject", UnsafeDuringActorConstruction = "true", BlueprintInternalUseOnly = "true"))
	static FTileManagerResult GetTileManager(UObject * WorldContextObject);
	
};
