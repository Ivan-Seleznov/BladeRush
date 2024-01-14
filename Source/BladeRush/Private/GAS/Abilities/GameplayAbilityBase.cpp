// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/GameplayAbilityBase.h"

#include "AbilitySystemComponent.h"
#include "Characters/BaseCharacter.h"

FActiveGameplayEffectHandle UGameplayAbilityBase::ApplyGameplayEffect(const FShooterGameplayEffect& GameplayEffect)
{
	const FGameplayAbilityActorInfo* ActorInfo = GetCurrentActorInfo();
	if (!ActorInfo) return FActiveGameplayEffectHandle();

	if (!GameplayEffect.GameplayEffect) return FActiveGameplayEffectHandle();
	
	const FGameplayEffectContextHandle EffectContextHandle;
	return ActorInfo->AbilitySystemComponent->ApplyGameplayEffectToSelf(GameplayEffect.GameplayEffect.GetDefaultObject(),GameplayEffect.Level,EffectContextHandle);
}

void UGameplayAbilityBase::RemoveGameplayEffectFromActiveHandle(FActiveGameplayEffectHandle& ActiveGameplayEffectHandle)
{
	const FGameplayAbilityActorInfo* ActorInfo = GetCurrentActorInfo();
	if (!ActorInfo) return;

	if (ActiveGameplayEffectHandle.IsValid())
	{
		ActorInfo->AbilitySystemComponent->RemoveActiveGameplayEffect(ActiveGameplayEffectHandle);
	}
}

const FGameplayTagContainer* UGameplayAbilityBase::GetCooldownTags() const
{
	FGameplayTagContainer* MutableTags = const_cast<FGameplayTagContainer*>(&TempCooldownTags);
	MutableTags->Reset(); // MutableTags writes to the TempCooldownTags on the CDO so clear it in case the ability cooldown tags change (moved to a different slot)
	const FGameplayTagContainer* ParentTags = Super::GetCooldownTags();
	if (ParentTags)
	{
		MutableTags->AppendTags(*ParentTags);
	}
	MutableTags->AppendTags(CooldownTags);
	return MutableTags;
}

void UGameplayAbilityBase::ApplyCooldown(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	UGameplayEffect* CooldownGE = GetCooldownGameplayEffect();
	if (CooldownGE)
	{
		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(CooldownGE->GetClass(), GetAbilityLevel());
		SpecHandle.Data.Get()->DynamicGrantedTags.AppendTags(CooldownTags);
		SpecHandle.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Cooldown")), CooldownDuration.GetValueAtLevel(GetAbilityLevel()));
		ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
	}
}


ABaseCharacter* UGameplayAbilityBase::GetCharacterFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<ABaseCharacter>(CurrentActorInfo->AvatarActor.Get()) : nullptr);
}

AController* UGameplayAbilityBase::GetControllerFromActorInfo() const
{
	const ACharacter* Character = GetCharacterFromActorInfo();
	return Character ? Character->GetController() : nullptr;
}
