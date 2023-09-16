// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ShooterMovementComponent.generated.h"

#define ERROR_VALUE -1.f;

class ABaseCharacter;

UENUM(BlueprintType)
enum ECustomMovementMode
{
	CMOVE_None		UMETA(Hidden),
	CMOVE_Slide		UMETA(DisplayName = "Slide"),
	CMOVE_Max		UMETA(Hidden)
};

UENUM(BlueprintType)
enum ETransitionName
{
	TNAME_None		UMETA(DispayName="NoneTransition"),
	TNAME_Mantle	UMETA(DispayName="MantleTransition"),
};

/**
 * Custom shooter movement component
 */
UCLASS()
class SIMPLESHOOTER_API UShooterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	class FSavedMove_Shooter : public FSavedMove_Character
	{
		/*SavedMove Flags*/
		uint8 Saved_bWantsToSprint:1;
		uint8 Saved_bPressedPlayerJump:1;
		
		/*SavedMove variable*/
		uint8 Saved_bWantsToSlide:1;
		
		/*Check can we combine two moves*/
		virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const override;
		
		virtual uint8 GetCompressedFlags() const override;

		/*Capture the state data of cmc*/
		virtual void SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData) override;
		virtual void PrepMoveFor(ACharacter* C) override;

		virtual void Clear() override;
	};

	class FNetworkPredictionDataClient_Shooter : public FNetworkPredictionData_Client_Character
	{
	public:
		FNetworkPredictionDataClient_Shooter(const UCharacterMovementComponent& ClientMovement);
		
		virtual FSavedMovePtr AllocateNewMove() override;
	};
	
public:
	
	UShooterMovementComponent();
	virtual void InitializeComponent() override;
	
	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;
	
	/*Client flag*/
	bool Safe_bWantsToSprint;
	
	bool Safe_bWantsToSlide;
	
	UFUNCTION(BlueprintPure)
	bool IsMovementMode(EMovementMode InMovementMode) const;

	UFUNCTION(BlueprintPure)
	bool IsCustomMovementMode(ECustomMovementMode InCustomMovementMode) const;

	virtual float GetMaxSpeed() const override;
	virtual float GetMaxBrakingDeceleration() const override;

	bool CanSprint() const;

	/*Sprint*/
	UPROPERTY(EditDefaultsOnly)
	float MaxSprintSpeed = 500.f;

	UPROPERTY(EditDefaultsOnly)
	float MaxCrouchingSprintSpeed = 350.f;
	
	/*Slide*/
	UPROPERTY(EditDefaultsOnly) float MinSlideSpeed=400.f;
	UPROPERTY(EditDefaultsOnly) float MaxSlideSpeed=400.f;
	UPROPERTY(EditDefaultsOnly) float SlideEnterImpulse=400.f;
	UPROPERTY(EditDefaultsOnly) float SlideGravityForce=4000.f;
	UPROPERTY(EditDefaultsOnly) float SlideFrictionFactor=.06f;
	UPROPERTY(EditDefaultsOnly) float BrakingDecelerationSliding=1000.f;

	bool CanSlide() const;
	
	UFUNCTION()
	FVector_NetQuantize GetMoveVector() const {return MoveVector;}
	
	UFUNCTION(Client, Unreliable)
	void Client_SetMoveVector(const FVector2D& NewValue);
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	virtual bool IsMovingOnGround() const override;
	virtual bool CanCrouchInCurrentState() const override;

protected:
	
	UPROPERTY(Transient) ABaseCharacter* ShooterCharacterOwner;

	UPROPERTY(Replicated)
	FVector_NetQuantize MoveVector;
	
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	ACharacter* GetDefaultCharacter() const;

	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;
	
	UFUNCTION(Server, Unreliable)
	void Server_SetMoveVector(const FVector2D& NewValue);

	virtual void UpdateCharacterStateBeforeMovement(float DeltaSeconds) override;

	virtual void PhysCustom(float deltaTime, int32 Iterations) override;

private:
	bool TryMantle() const;
	
	void EnterSlide();
	void ExitSlide();

	void PhysSlide(float DeltaTime, int32 Iterations);
	bool GetSlideSurface(FHitResult& Hit) const;
};
