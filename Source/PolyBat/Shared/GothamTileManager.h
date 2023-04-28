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

	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	int GetNumberOfLanes() const { return NumberOfLanes; }
	
	UFUNCTION()
	int GetLaneWidth() const { return LaneWidth; }

	UFUNCTION()
	float GetTileSpeed() const { return TileSpeed;}

protected:
	UPROPERTY(VisibleAnywhere, Category="PolyBat|Components")
	USceneComponent *Root;
	
	UPROPERTY(VisibleDefaultsOnly, Category="PolyBat|Components")
	class UBoxComponent *LevelBounds;

private:
	UPROPERTY()
	TArray<AGothamTile*> GothamTiles;

	UPROPERTY(EditDefaultsOnly, Category="PolyBat")
	TSubclassOf<AGothamTile> GothamTileClass;

	int NumberOfLanes;

	UPROPERTY(EditDefaultsOnly, Category="PolyBat")
	float LaneWidth;

	UPROPERTY(EditAnywhere, Category="PolyBat")
	float TileSpeed;

	UPROPERTY(EditAnywhere, Category="PolyBat", Meta = (MakeEditWidget = true))
	FVector OutOfBoundBoxLocation;

	UPROPERTY(EditAnywhere, Category="PolyBat", Meta = (MakeEditWidget = true))
	FVector OutOfBoundBoxScale;
	

};
