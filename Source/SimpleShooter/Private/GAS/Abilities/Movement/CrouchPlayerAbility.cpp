// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/Movement/CrouchPlayerAbility.h"

#include "Characters/BaseCharacter.h"

void UCrouchPlayerAbility::InputReleased(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
	
	EndAbility(Handle,ActorInfo,ActivationInfo, true, true);
}

void UCrouchPlayerAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	ABaseCharacter* Character = GetCharacterFromActorInfo();
	if (!Character) return;

	if (!Character->CanCrouch()) return;
	
	Character->Crouch();
	UE_LOG(LogTemp,Display,TEXT("ActivateCrouch"));
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
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
	if (PrevMovementMode == MOVE_Walking) EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
