// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Scroller.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class POLYBAT_API UScroller : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UScroller();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	class AGothamTileManager * TileManager;
	
	UPROPERTY()
	FVector DestinationOffset;

		
};
