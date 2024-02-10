// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventoryItemDefinition.h"
#include "WeaponDataFragment.generated.h"

class UMagazineItemInstance;

/**
 * Weapon data fragment. Contains necessary data for weapons
 */
UCLASS()
class BLADERUSH_API UWeaponDataFragment : public UInventoryItemFragment
{
	GENERATED_BODY()

public:
	TWeakPtr<UMagazineItemInstance*> MagazineItemInstance;
};
