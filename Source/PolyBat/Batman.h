// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "Batman.generated.h"

UENUM(BlueprintType)
enum class EOtherPath : uint8
{
	Middle UMETA(DisplayName = "Middle"),
	Left UMETA(DisplayName = "Left"),
	Right UMETA(DisplayName = "Right"),
	NotSet UMETA(Displayname = "NotSet")
};



UCLASS()
class POLYBAT_API ABatman : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABatman();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveRight(const FInputActionValue& Value);
	void MoveLeft(const FInputActionValue& Value);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PolyBat|EnhancedInput")
	class UInputMappingContext * MappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PolyBat|EnhancedInput")
	class UInputAction * MoveRightAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PolyBat|EnhancedInput")
	class UInputAction * MoveLeftAction;

private:
	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent * Capsule;

	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent * BatmanMesh;

	UPROPERTY(VisibleAnywhere, Category="PolyBat|Components")
	class USpringArmComponent *SpringArm;

	UPROPERTY(VisibleAnywhere, Category="PolyBat|Components")
	class UCameraComponent *Camera;

	UPROPERTY(EditAnywhere, Category="PolyBat|Components")
	float MoveDistance = 300.f;

	FVector StartLocation;
	FVector LeftLocation;
	FVector MiddleLocation;
	FVector RightLocation;

	UPROPERTY(EditAnywhere, Category="PolyBat|Components")
	FVector MovementVelocity = FVector(0, 3000, 0);
	

	UPROPERTY(EditAnywhere, Category="PolyBat|Components")
	float MovementSpeed = 20.0f;

	UPROPERTY()
	EOtherPath CurrentPath;

	UPROPERTY()
	EOtherPath PreviousPath;

	UPROPERTY()
	bool bIsMoving;

	UPROPERTY()
	float MovementAmount = 300.f;
};
