// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Bat.generated.h"

UENUM(BlueprintType)
enum class EPath : uint8
{
	Middle UMETA(DisplayName = "Middle"),
	Left UMETA(DisplayName = "Left"),
	Right UMETA(DisplayName = "Right"),
	NotSet UMETA(Displayname = "NotSet")
};



UCLASS()
class POLYBAT_API ABat : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="PolyBat|Components")
	class USpringArmComponent *SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="PolyBat|Components")
	class UCameraComponent *Camera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PolyBat|EnhancedInput")
	class UInputMappingContext * MappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PolyBat|EnhancedInput")
	class UInputAction * MoveRightAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PolyBat|EnhancedInput")
	class UInputAction * MoveLeftAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PolyBat|EnhancedInput")
	class UInputAction * JumpAction;

private:
	UPROPERTY()
	bool bReadyToRun;
	
	UPROPERTY()
	EPath CurrentPath;

	UPROPERTY()
	EPath PreviousPath;

	UPROPERTY()
	bool bSwitchingLanes;

	UPROPERTY(EditAnywhere, Category="PolyBat|Components")
	float MoveDistance = 300.f;
	
	UPROPERTY(EditAnywhere, Category="PolyBat|Components")
	FVector MovementVelocity = FVector(750, 3000, 0);
	
	UPROPERTY(EditAnywhere, Category="PolyBat|Components")
	float MovementSpeed = 20.0f;

	FVector StartLocation;
	FVector LeftLocation;
	FVector MiddleLocation;
	FVector RightLocation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void MoveRight(const FInputActionValue& Value);
	void MoveLeft(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="PolyBat|Functions")
	void MakeJump(const FInputActionValue & Value);

public:
	// Sets default values for this character's properties
	ABat();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return SpringArm; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return Camera; }

	FORCEINLINE void SetReadyToRun(const bool Value) { bReadyToRun = Value;}
	UFUNCTION(BlueprintPure, Category="PolyBat")
	FORCEINLINE bool GetReadyToRun() const { return bReadyToRun; }
	

};
