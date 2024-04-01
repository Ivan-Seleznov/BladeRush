// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Weapons/WeaponTypes.h"
#include "WeaponTypesDataAsset.generated.h"

class UInventoryItemDefinition;

USTRUCT(BlueprintType)
struct FLoadoutWeapons
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TMap<FName,TSoftClassPtr<UInventoryItemDefinition>> LoadoutWeapons;
};

/**
 * Weapon types data asset
 */
UCLASS()
class BLADERUSH_API UWeaponTypesDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	const TMap<EWeaponType,FLoadoutWeapons>& GetWeaponTypes() const;
private:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(AllowPrivateAccess))
	TMap<EWeaponType,FLoadoutWeapons> WeaponTypes;
};
