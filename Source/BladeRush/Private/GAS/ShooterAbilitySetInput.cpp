// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/ShooterAbilitySetInput.h"

#include "Input/ShooterAbilityTagConfig.h"

void UShooterAbilitySetInput::SetupAbilitySpec(FGameplayAbilitySpec& AbilitySpec,
                                               const FAbilitySet_GameplayAbility& AbilityToGrant, UObject* SourceObject) const
{
	Super::SetupAbilitySpec(AbilitySpec, AbilityToGrant, SourceObject);
	
	if (AbilityTagConfig)
	{
		const FGameplayTag InputTag = AbilityTagConfig->GetTagForAbility(AbilityToGrant.Ability);
		if (InputTag != FGameplayTag::EmptyTag)
		{
			AbilitySpec.DynamicAbilityTags.AddTag(InputTag);
		}
	}
}
