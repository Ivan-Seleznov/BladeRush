// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/Movement/MovementAbilityBase.h"

#include "AbilitySystemComponent.h"
#include "Characters/BaseCharacter.h"

void UMovementAbilityBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ABaseCharacter* Character = GetCharacterFromActorInfo();
	if (!Character) return;

	if (AbilityTagEffect.GameplayEffect)
	{
		const FGameplayEffectContextHandle EffectHandle;
		ActiveAbilityTagEffectHandle = ActorInfo->AbilitySystemComponent->ApplyGameplayEffectToSelf(AbilityTagEffect.GameplayEffect.GetDefaultObject(),
			AbilityTagEffect.Level, EffectHandle);	
	}
}

void UMovementAbilityBase::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
