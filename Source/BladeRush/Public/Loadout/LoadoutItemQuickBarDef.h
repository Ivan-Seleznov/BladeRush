// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LoadoutEquipmentDefinition.h"
#include "LoadoutItemQuickBarDef.generated.h"

class UInventoryItemDefinition;

/**
 * Loadout Item To QuickBar Definition. Contains QuickBar slot index and TSoftClassPtr<UInventoryItemDefinition> 
 */
UCLASS()
class BLADERUSH_API ULoadoutItemQuickBarDef : public ULoadoutDefinitionBase
{
	GENERATED_BODY()

public:
	int32 GetSlotIndex() const {return SlotIndex;}

	TSoftClassPtr<UInventoryItemDefinition> GetItemDefinitionSoftClass() const {return ItemDefinitionSoftClass;}
	
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int32 SlotIndex;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSoftClassPtr<UInventoryItemDefinition> ItemDefinitionSoftClass;
};
