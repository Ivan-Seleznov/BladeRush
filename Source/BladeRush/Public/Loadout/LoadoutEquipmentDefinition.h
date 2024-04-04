// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LoadoutDefinitionBase.h"
#include "LoadoutEquipmentDefinition.generated.h"

class UEquipmentDefinition;

/**
 * Loadout definition wich contains TSoftClassPtr<UEquipmentDefinition>
 */
UCLASS()
class BLADERUSH_API ULoadoutEquipmentDefinition : public ULoadoutDefinitionBase
{
	GENERATED_BODY()

public:
	TSoftClassPtr<UEquipmentDefinition> GetEquipmentDefinitionSoftClass() const {return EquipmentDefinitionSoftClass;}
	
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSoftClassPtr<UEquipmentDefinition> EquipmentDefinitionSoftClass;
};
