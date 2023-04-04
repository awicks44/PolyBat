// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GothamTile.generated.h"

UENUM(BlueprintType) 
enum class EMovementState : uint8
{
	SetInLocation,
	ChangingLocation
};

UCLASS()
class POLYBAT_API AGothamTile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGothamTile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void AdjustPosition(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

private:
	UPROPERTY(VisibleAnywhere, Category="PolyBat|Components")
	USceneComponent *Root;
	
	UPROPERTY(VisibleAnywhere, Category="PolyBat|Components")
	class UStaticMeshComponent * Mesh;

	UPROPERTY(VisibleDefaultsOnly, Category="PolyBat|Components")
	class UBoxComponent *Trigger;

	UPROPERTY(VisibleAnywhere, Category="PolyBat|Components")
	EMovementState MovementState;

};
