// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GothamTileManager.generated.h"

class AGothamTile;
UCLASS()
class POLYBAT_API AGothamTileManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGothamTileManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	

private:
	UPROPERTY()
	TArray<AGothamTile*> GothamTiles;

	UPROPERTY(EditDefaultsOnly, Category="PolyBat")
	TSubclassOf<AGothamTile> GothamTileClass;

};