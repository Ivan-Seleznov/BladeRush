// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LoadoutDefinitionBase.h"
#include "LoadoutItemDefinition.generated.h"

class UInventoryItemDefinition;

/**
 * Loadout definition wich contains TSoftClassPtr<UInventoryItemDefinitio>
 */
UCLASS()
class BLADERUSH_API ULoadoutItemDefinition : public ULoadoutDefinitionBase
{
	GENERATED_BODY()

public:
	TSoftClassPtr<UInventoryItemDefinition> GetItemDefinitionSoftClass() const {return ItemDefinitionSoftClass;}
	
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSoftClassPtr<UInventoryItemDefinition> ItemDefinitionSoftClass;
};
