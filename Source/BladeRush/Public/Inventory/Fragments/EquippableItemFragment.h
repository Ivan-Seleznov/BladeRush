// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventoryItemDefinition.h"
#include "EquippableItemFragment.generated.h"

class UEquipmentDefinition;

UCLASS()
class BLADERUSH_API UEquippableItemFragment : public UInventoryItemFragment
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadOnly)	
	TSubclassOf<UEquipmentDefinition> EquipmentDefinitionClass;
};
