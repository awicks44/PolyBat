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
	
	void Init();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="PolyBat|Functions")
	void MoveRight(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="PolyBat|Functions")
	void MoveLeft(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent, Category="PolyBat|Functions")
	void Kick(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent, Category="PolyBat|Functions")
	void Punch(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="PolyBat|Functions")
	void MakeJump(const FInputActionValue & Value);

	UFUNCTION(BlueprintCallable, Category="PolyBat|Functions")
	void SetMoving(const bool Value) { bIsMoving = Value;}

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PolyBat|EnhancedInput")
	class UInputAction * JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PolyBat|EnhancedInput")
	class UInputAction * KickAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PolyBat|EnhancedInput")
	class UInputAction * PunchAction;

	UPROPERTY(BlueprintReadOnly, Category="PolyBat")
	int CurrentLane;

	UPROPERTY(BlueprintReadOnly, Category="PolyBat")
	float LaneWidth;

	UPROPERTY(BlueprintReadOnly, Category="PolyBat")
	float TargetLocation;

	UPROPERTY(BlueprintReadOnly, Category="PolyBat")
	bool bIsMoving;

private:
	UPROPERTY(VisibleAnywhere, Category="PolyBat|Components")
	class UCapsuleComponent * Capsule;

	UPROPERTY(VisibleAnywhere, Category="PolyBat|Components")
	class USkeletalMeshComponent * BatmanMesh;

	UPROPERTY(VisibleAnywhere, Category="PolyBat|Components")
	class USpringArmComponent *SpringArm;

	UPROPERTY(VisibleAnywhere, Category="PolyBat|Components")
	class UCameraComponent *Camera;
	
	UPROPERTY()
	class AGothamTileManager * TileManager;

	
};
