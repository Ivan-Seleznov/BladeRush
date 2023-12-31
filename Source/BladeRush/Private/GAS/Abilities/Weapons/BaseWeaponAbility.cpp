// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/Weapons/BaseWeaponAbility.h"
#include "Weapons/WeaponItemInstance.h"

UWeaponItemInstance* UBaseWeaponAbility::GetWeaponInstance() const
{
	return Cast<UWeaponItemInstance>(GetAssociatedEquipmentInstance());
}
