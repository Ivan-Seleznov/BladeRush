// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/GameplayAbilityBase.h"
#include "GameplayAbilityFromEquipment.generated.h"

class UInventoryItemInstance;
class UEquipmentInstance;
/**
 * 
 */
UCLASS()
class BLADERUSH_API UGameplayAbilityFromEquipment : public UGameplayAbilityBase
{
	GENERATED_BODY()
public:
	UGameplayAbilityFromEquipment(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UEquipmentInstance* GetAssociatedEquipmentInstance() const;
	UInventoryItemInstance* GetAssociatedInventoryItemInstance() const;

	
};
