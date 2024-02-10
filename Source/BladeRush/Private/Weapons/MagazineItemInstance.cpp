// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/MagazineItemInstance.h"

int32 UMagazineItemInstance::AddCartridgeToMagazine(const int32& CartridgeCount)
{
	int32 AmmoToAdd = FMath::Min(CartridgeCount, MaxAmmoCount - CurrentAmmoCount);
	CurrentAmmoCount += AmmoToAdd;

	int32 RemainingCartridges = CartridgeCount - AmmoToAdd;

	if (CurrentAmmoCount > 0 && bOutOfAmmo)
	{
		bOutOfAmmo = false;
	}
	
	return RemainingCartridges;
}

int32 UMagazineItemInstance::RemoveCartridge()
{
	if (CurrentAmmoCount > 0)
	{
		CurrentAmmoCount--;
	}

	if (CurrentAmmoCount <= 0)
	{
		bOutOfAmmo = true;
	}

	return CurrentAmmoCount;
}
