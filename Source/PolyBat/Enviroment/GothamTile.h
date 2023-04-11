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

UENUM(BlueprintType) 
enum class EGothamTriggerState : uint8
{
	Trigger_Hit,
	Trigger_Waiting,
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

	UFUNCTION()
	void ResetMovementState() { MovementState = EMovementState::SetInLocation;}

	UFUNCTION()
	bool ExitPointReached() const { return GothamTriggerState == EGothamTriggerState::Trigger_Hit; }

	UFUNCTION()
	void ResetTriggerState() { GothamTriggerState = EGothamTriggerState::Trigger_Waiting;}

	UFUNCTION()
	bool MustChangePosition() const { return MovementState == EMovementState::ChangingLocation; }

	UFUNCTION()
	void ChangePosition();

	UFUNCTION()
	int GetIndex() const { return Index; }

	UFUNCTION()
	int GetSiblingIndex() const { return SiblingIndex; }

	UFUNCTION()
	void SetIndex(const int Value) { Index = Value;}

	UFUNCTION()
	void SetSiblingIndex(const int Value) { SiblingIndex = Value; }

	

private:
	UFUNCTION()
	void ExitTriggerOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

private:
	UPROPERTY(VisibleAnywhere, Category="PolyBat|Components")
	USceneComponent *Root;
	
	UPROPERTY(VisibleAnywhere, Category="PolyBat|Components")
	class UStaticMeshComponent * Mesh;

	UPROPERTY(VisibleDefaultsOnly, Category="PolyBat|Components")
	class UBoxComponent *Trigger;

	UPROPERTY(VisibleAnywhere, Category="PolyBat|Props")
	EMovementState MovementState;

	UPROPERTY(VisibleAnywhere, Category="PolyBat|Props")
	EGothamTriggerState GothamTriggerState;

	UPROPERTY()
	int Index;

	UPROPERTY()
	int SiblingIndex;

};
