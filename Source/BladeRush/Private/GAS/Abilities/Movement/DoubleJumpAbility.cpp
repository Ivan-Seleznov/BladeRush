// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/Movement/DoubleJumpAbility.h"

#include "BladeRushLogs.h"
#include "Characters/BaseCharacter.h"

void UDoubleJumpAbility::InputReleased(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);

	EndAbility(Handle,ActorInfo,ActivationInfo,true,true);
}

bool UDoubleJumpAbility::CanActivateMovementAbility(ABaseCharacter* Character,
	UShooterMovementComponent* ShooterMovementComponent) const
{
	DEBUG_LOG("CharacterJumpCount: %i",Character->JumpCurrentCount);
	return Character->JumpCurrentCount > 0;
}

void UDoubleJumpAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                         const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ABaseCharacter* Character = GetCharacterFromActorInfo();
	if (!Character)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
	}

	if (Character->IsLocallyControlled())
	{
		Character->Jump();
	}
}

void UDoubleJumpAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	ABaseCharacter* Character = GetCharacterFromActorInfo();
	if (Character && Character->IsLocallyControlled())
	{
		Character->StopJumping();
	}
}
