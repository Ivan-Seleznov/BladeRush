// Fill out your copyright notice in the Description page of Project Settings.

#include "Input/ShooterAbilityTagConfig.h"
#include "GameplayTagContainer.h"

FGameplayTag UShooterAbilityTagConfig::GetTagForAbility(TSubclassOf<UGameplayAbility> Ability) const
{
	if (!AbilityInputs.Contains(Ability))
	{
		return FGameplayTag::EmptyTag;
	}
	return AbilityInputs[Ability];
}
