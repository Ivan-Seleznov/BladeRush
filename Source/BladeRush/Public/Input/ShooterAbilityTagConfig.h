// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbility.h"

#include "ShooterAbilityTagConfig.generated.h"

/**
 * 
 */
UCLASS()
class BLADERUSH_API UShooterAbilityTagConfig : public UDataAsset
{
	GENERATED_BODY()
public:
	FGameplayTag GetTagForAbility(TSubclassOf<UGameplayAbility> Ability) const;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Input",meta = (AllowPrivateAccess = "true"))
	TMap<TSubclassOf<UGameplayAbility>, FGameplayTag> AbilityInputs;
};
