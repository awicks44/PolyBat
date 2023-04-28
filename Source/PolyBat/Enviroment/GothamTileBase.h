// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GothamTileBase.generated.h"

UCLASS()
class POLYBAT_API AGothamTileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGothamTileBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, Category="PolyBat|Components")
	USceneComponent *Root;
	
	UPROPERTY(VisibleAnywhere, Category="PolyBat|Components")
	class UStaticMeshComponent * Mesh;
	
};
