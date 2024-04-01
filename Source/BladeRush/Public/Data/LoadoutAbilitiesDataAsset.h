// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LoadoutAbilitiesDataAsset.generated.h"

class UShooterAbilitySet;


/**
 * Loadout Abilities Data asset. Contains ability set and ability set name
 */
UCLASS()
class BLADERUSH_API ULoadoutAbilitiesDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	const TMap<FName,TSoftObjectPtr<UShooterAbilitySet>>& GetLoadoutAbilities() const;
	
private:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(AllowPrivateAccess))
	TMap<FName,TSoftObjectPtr<UShooterAbilitySet>> LoadoutAbilities;
};
