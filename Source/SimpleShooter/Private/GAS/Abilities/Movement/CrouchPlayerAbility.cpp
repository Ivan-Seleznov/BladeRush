// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/Movement/CrouchPlayerAbility.h"

#include "Characters/BaseCharacter.h"
#include "Characters/Components/ShooterMovementComponent.h"


UCrouchPlayerAbility::UCrouchPlayerAbility()
{
	bCrouching = false;
}

void UCrouchPlayerAbility::InputPressed(const FGameplayAbilitySpecHandle Handle,
                                        const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	ABaseCharacter* Character = GetCharacterFromActorInfo();
	if (!Character) return;
	
	if (bCrouching)
	{
		EndAbility(Handle,ActorInfo,ActivationInfo,true,true);
		return;
	}

	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
}

bool UCrouchPlayerAbility::CanActivateMovementAbility(ABaseCharacter* Character,
	UShooterMovementComponent* ShooterMovementComponent) const
{
	return Character->CanCrouch();
}

void UCrouchPlayerAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ABaseCharacter* Character = GetCharacterFromActorInfo();
	if (!Character) return;

	UShooterMovementComponent* MovementComponent = Character->GetShooterMovementComponent();
	if (!MovementComponent) return;
	
	Character->Crouch();
	
	bCrouching = true;
}

void UCrouchPlayerAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
	ABaseCharacter* Character = GetCharacterFromActorInfo();
	if (!Character) return;

	
	Character->UnCrouch();
}

void UCrouchPlayerAbility::MovementModeChanged(ACharacter* Character, EMovementMode PrevMovementMode,
	uint8 PreviousCustomMode)
{
	/*if (PrevMovementMode == MOVE_Walking) EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);*/
}
