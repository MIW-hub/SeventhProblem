// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/FloatingPawnMovement.h" 
#include "SeventhPawnFuction.generated.h"
class InputActionValue;
class USpringArmComponent;
class UCameraComponent;
class ASeventhPlayerController;


class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
UCLASS()
class SEVENTHPROBLEM_API ASeventhPawnFuction : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASeventhPawnFuction();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	/*카메라 관련 변수*/
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	UFloatingPawnMovement* MovementComp;

	float NormalSpeed;
	float SprintSpeed;
	float SprintSpeedMultiplier;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;
	/*여기까지 카메라*/

	/*콜리전,스컬레토 메쉬*/
	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* MeshComp;
	/*여기까지 콜리전,스컬레톤메쉬*/
public:	
	// Called every frame

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext = nullptr;


	// 이동, 룩 행동 그 자체
	UPROPERTY()
	float MoveSpeed = 40.0f;
	UPROPERTY()
	float RotationSpeed = 2.0f;

	// 실제로 캐릭터를 움직일 함수
	UFUNCTION()
	void Move(const FInputActionValue& Value);
	UFUNCTION()
	void Look(const FInputActionValue& Value);
	


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
