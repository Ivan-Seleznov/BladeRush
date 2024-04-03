// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LoadoutDefinitionBase.h"
#include "LoadoutAbilitiesDefinition.generated.h"

class UShooterAbilitySet;
/**
 * Loadout definition wich contains TSoftObjectPtr<UShooterAbilitySet>
 */
UCLASS()
class BLADERUSH_API ULoadoutAbilitiesDefinition : public ULoadoutDefinitionBase
{
	GENERATED_BODY()

public:
	TSoftObjectPtr<UShooterAbilitySet> GetShooterAbilitySetSoft() const {return ShooterAbilitySetSoft;}
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSoftObjectPtr<UShooterAbilitySet> ShooterAbilitySetSoft;
};
