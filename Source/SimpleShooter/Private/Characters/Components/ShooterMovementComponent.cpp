// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Components/ShooterMovementComponent.h"

#include "GameFramework/Character.h"

bool UShooterMovementComponent::FSavedMove_Shooter::CanCombineWith(const FSavedMovePtr& NewMove,
                                                                   ACharacter* InCharacter, float MaxDelta) const
{
	const FSavedMove_Shooter* NewShooterMove = static_cast<FSavedMove_Shooter*>(NewMove.Get());

	if (Saved_bWantsToSprint != NewShooterMove->Saved_bWantsToSprint)
	{
		return false;
	}
	return FSavedMove_Character::CanCombineWith(NewMove, InCharacter, MaxDelta);
}

uint8 UShooterMovementComponent::FSavedMove_Shooter::GetCompressedFlags() const
{
	/*need client to replicate data*/
	uint8 Result = FSavedMove_Character::GetCompressedFlags();

	if (Saved_bWantsToSprint) Result |= FLAG_Custom_0;
	
	return Result;
}

void UShooterMovementComponent::FSavedMove_Shooter::SetMoveFor(ACharacter* C, float InDeltaTime,
	FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData)
{
	FSavedMove_Character::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);

	UShooterMovementComponent* ShooterMovementComponent = Cast<UShooterMovementComponent>(C->GetCharacterMovement());
	Saved_bWantsToSprint = ShooterMovementComponent->Safe_bWantsToSprint;
}

void UShooterMovementComponent::FSavedMove_Shooter::PrepMoveFor(ACharacter* C)
{
	FSavedMove_Character::PrepMoveFor(C);

	UShooterMovementComponent* ShooterMovementComponent = Cast<UShooterMovementComponent>(C->GetCharacterMovement());
	ShooterMovementComponent->Safe_bWantsToSprint = Saved_bWantsToSprint;
}

void UShooterMovementComponent::FSavedMove_Shooter::Clear()
{
	FSavedMove_Character::Clear();

	Saved_bWantsToSprint = 0;
}

UShooterMovementComponent::FNetworkPredictionDataClient_Shooter::FNetworkPredictionDataClient_Shooter(const UCharacterMovementComponent& ClientMovement)
: FNetworkPredictionData_Client_Character(ClientMovement)
{
}

FSavedMovePtr UShooterMovementComponent::FNetworkPredictionDataClient_Shooter::AllocateNewMove()
{
	return FSavedMovePtr(new FSavedMove_Shooter());
}

UShooterMovementComponent::UShooterMovementComponent()
{
}

FNetworkPredictionData_Client* UShooterMovementComponent::GetPredictionData_Client() const
{
	check(PawnOwner)

	if (!ClientPredictionData)
	{
		UShooterMovementComponent* MutableThis = const_cast<UShooterMovementComponent*>(this);

		MutableThis->ClientPredictionData = new FNetworkPredictionDataClient_Shooter(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
	}
	return ClientPredictionData;
}

bool UShooterMovementComponent::IsMovementMode(EMovementMode InMovementMode) const
{
	return InMovementMode == MovementMode;
}

float UShooterMovementComponent::GetMaxSpeed() const
{
	if (IsMovementMode(MOVE_Walking) && Safe_bWantsToSprint && !IsCrouching()) return MaxSprintSpeed;

	if (MovementMode != MOVE_Custom)
	{
		return Super::GetMaxSpeed();
	}

	/*switch (CustomMovementMode)
	{

	default:
		UE_LOG(LogTemp,Fatal,TEXT("Invalid Movement Mode (getmaxspeed)"));
		return -1.f;
	}*/
	
	return ERROR_VALUE;
}

bool UShooterMovementComponent::CanSprint() const
{
	return IsMovementMode(MOVE_Walking) && !IsCrouching();
}

void UShooterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	Safe_bWantsToSprint = (Flags & FSavedMove_Shooter::FLAG_Custom_0) != 0;
}
