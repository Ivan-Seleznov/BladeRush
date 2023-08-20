// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/ShooterAbilitySet.h"
#include "ShooterAbilitySetInput.generated.h"

class UShooterAbilityTagConfig;
/**
 * AbilitySet that contains UShooterAbilityTagConfig and add input tags to DynamicAbilityTags
 */
UCLASS()
class SIMPLESHOOTER_API UShooterAbilitySetInput : public UShooterAbilitySet
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta=(TitleProperty=AttributeSet))
	UShooterAbilityTagConfig* AbilityTagConfig;
	
	virtual void SetupAbilitySpec(FGameplayAbilitySpec& AbilitySpec, const FAbilitySet_GameplayAbility& AbilityToGrant, UObject* SourceObject) const override;
};
