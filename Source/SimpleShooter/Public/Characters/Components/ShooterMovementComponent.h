// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ShooterMovementComponent.generated.h"

#define ERROR_VALUE -1.f;

class ABaseCharacter;

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
	
	UFUNCTION(BlueprintPure)
	bool IsMovementMode(EMovementMode InMovementMode) const;
	
	virtual float GetMaxSpeed() const override;
	
	bool CanSprint() const;
	float MaxSprintSpeed = 700.f;

	UFUNCTION()
	FVector_NetQuantize GetMoveVector() const {return MoveVector;}
	
	UFUNCTION(Client, Unreliable)
	void Client_SetMoveVector(const FVector2D& NewValue);
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	
	UPROPERTY(Transient) ABaseCharacter* ShooterCharacterOwner;

	UPROPERTY(Replicated)
	FVector_NetQuantize MoveVector;
	
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	ACharacter* GetDefaultCharacter() const;

	UFUNCTION(Server, Unreliable)
	void Server_SetMoveVector(const FVector2D& NewValue);
private:

};
