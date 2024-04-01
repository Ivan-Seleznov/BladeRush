// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LoadoutEquipmentDataAsset.generated.h"

class UInventoryItemDefinition;

/**
 * Loadout equipment DataAsset. Contains 
 */
UCLASS()
class BLADERUSH_API ULoadoutEquipmentDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	const TMap<FName,TSoftClassPtr<UInventoryItemDefinition>>& GetLoadoutEquipment() const;
	
private:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(AllowPrivateAccess))
	TMap<FName,TSoftClassPtr<UInventoryItemDefinition>> LoadoutEquipment;
	
};
