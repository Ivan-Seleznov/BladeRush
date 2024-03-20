// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/GameplayAbilityFromEquipment.h"
#include "BaseWeaponAbility.generated.h"

class UWeaponItemInstance;

/**
 * Base weapon ability class
 */
UCLASS()
class BLADERUSH_API UBaseWeaponAbility : public UGameplayAbilityFromEquipment
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure)
	UWeaponItemInstance* GetWeaponInstance() const;

protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	bool bEnableDebug = true;
};
