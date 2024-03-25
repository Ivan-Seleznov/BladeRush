// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/GameplayAbilityFromEquipment.h"

#include "Equipment/EquipmentInstance.h"
#include "Inventory/InventoryItemInstance.h"

UGameplayAbilityFromEquipment::UGameplayAbilityFromEquipment(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

UEquipmentInstance* UGameplayAbilityFromEquipment::GetAssociatedEquipmentInstance() const
{
	if (FGameplayAbilitySpec* Spec = UGameplayAbility::GetCurrentAbilitySpec())
	{
		return Cast<UEquipmentInstance>(Spec->SourceObject.Get());
	}

	return nullptr;
}

UInventoryItemInstance* UGameplayAbilityFromEquipment::GetAssociatedInventoryItemInstance() const
{
	if (UEquipmentInstance* EquipmentInstance = GetAssociatedEquipmentInstance())
	{
		return Cast<UInventoryItemInstance>(EquipmentInstance->GetInstigator());
	}
	return nullptr;
}

