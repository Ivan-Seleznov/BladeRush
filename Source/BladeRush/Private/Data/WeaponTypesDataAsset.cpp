// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/WeaponTypesDataAsset.h"

const TMap<EWeaponType,FLoadoutWeapons>& UWeaponTypesDataAsset::GetWeaponTypes() const
{
	return WeaponTypes;
}
