// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponItemInstance.h"

UWeaponItemInstance::UWeaponItemInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UWeaponItemInstance::OnEquipped()
{
	Super::OnEquipped();
}

void UWeaponItemInstance::OnUnequipped()
{
	Super::OnUnequipped();
}

AActor* UWeaponItemInstance::GetSpawnedWeaponActor() const
{
	const int32 WeaponIndex = 0;

	if (SpawnedActors.IsValidIndex(WeaponIndex))
	{
		return SpawnedActors[WeaponIndex];
	}

	return nullptr;
}
