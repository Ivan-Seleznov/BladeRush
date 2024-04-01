// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/LoadoutAbilitiesDataAsset.h"

const TMap<FName,TSoftObjectPtr<UShooterAbilitySet>>& ULoadoutAbilitiesDataAsset::GetLoadoutAbilities() const
{
	return LoadoutAbilities;
}
