// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/Movement/JumpPlayerAbility.h"

#include "AbilitySystemComponent.h"
#include "Characters/BaseCharacter.h"
#include "GAS/Attributes/AttributeStamina.h"

void UJumpPlayerAbility::InputReleased(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
	EndAbility(Handle,ActorInfo,ActivationInfo,true,true);
}

void UJumpPlayerAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                         const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ABaseCharacter* Character = GetCharacterFromActorInfo();
	if (!Character) return;

	if (Character->bIsCrouched)
	{
		Character->UnCrouch();
	}
	
	Character->Jump();

	FGameplayEffectContextHandle GameplayEffectContextHandle;
	Character->GetAbilitySystemComponent()->ApplyGameplayEffectToSelf(DebuffEffect.GameplayEffect.GetDefaultObject(), DebuffEffect.Level, GameplayEffectContextHandle);

}

void UJumpPlayerAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	ABaseCharacter* Character = GetCharacterFromActorInfo();
	if (!Character) return;

	Character->StopJumping();
}

bool UJumpPlayerAbility::CanActivateMovementAbility(ABaseCharacter* Character,
                                                    UShooterMovementComponent* ShooterMovementComponent) const
{
	const UAttributeStamina* AttributeStamina = UAttributeStamina::Find(Character->GetAbilitySystemComponent());

	const float StaminaPoints = AttributeStamina->GetStaminaPoints();
	const float MaxStaminaPoints = AttributeStamina->GetMaxStaminaPoints();
	
	return StaminaPoints > MaxStaminaPoints * BlockJumpMultiplier /*&& Character->CanJump()*/;
}
