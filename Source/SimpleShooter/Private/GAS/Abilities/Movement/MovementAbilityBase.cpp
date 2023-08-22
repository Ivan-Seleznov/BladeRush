// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/Movement/MovementAbilityBase.h"

#include "AbilitySystemComponent.h"
#include "Characters/BaseCharacter.h"
#include "GAS/Attributes/AttributeStamina.h"

void UMovementAbilityBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ABaseCharacter* Character = GetCharacterFromActorInfo();
	if (!Character) return;

	UShooterMovementComponent* MovementComponent = Character->GetShooterMovementComponent();
	if (!MovementComponent) return;

	if (AbilityTagEffect.GameplayEffect)
	{
		const FGameplayEffectContextHandle EffectHandle;
		ActiveAbilityTagEffectHandle = ActorInfo->AbilitySystemComponent->ApplyGameplayEffectToSelf(AbilityTagEffect.GameplayEffect.GetDefaultObject(),
			AbilityTagEffect.Level, EffectHandle);	
	}
}

bool UMovementAbilityBase::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	ABaseCharacter* Character = Cast<ABaseCharacter>(ActorInfo->AvatarActor.Get());
	if (!Character) return false;

	UShooterMovementComponent* MovementComponent = Character->GetShooterMovementComponent();
	if (!MovementComponent) return false;
	
	if (!CanActivateMovementAbility(Character,MovementComponent)) return false;
	
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}
	
	return true;
}

bool UMovementAbilityBase::CanActivateMovementAbility(ABaseCharacter* Character,
	UShooterMovementComponent* ShooterMovementComponent) const
{
	return true;
}

void UMovementAbilityBase::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                      bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
