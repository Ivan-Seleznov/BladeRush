// Fill out your copyright notice in the Description page of Project Settings.


#include "Loadout/LoadoutComponent.h"

#include "Characters/BaseCharacter.h"
#include "Equipment/EquipmentManagerComponent.h"
#include "Inventory/InventoryManagerComponent.h"
#include "Inventory/QuickBarComponent.h"


ULoadoutComponent::ULoadoutComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void ULoadoutComponent::SetCurrentLoadout(const FCharacterLoadout& NewLoadout, bool bTryApplyLoadout)
{
	CurrentLoadout = NewLoadout;

	if (bTryApplyLoadout)
	{
		TryApplyCurrentLoadout();
	}
}

bool ULoadoutComponent::TryApplyCurrentLoadout()
{
	ABaseCharacter* Character = GetPawn<ABaseCharacter>();
	if (!Character || CurrentLoadout.IsEmpty())
	{
		return false;
	}
	
	ApplyAbilities(Character);
	ApplyEquipment(Character);
	ApplyQuickBarEquipment(Character);

	return true;
}

void ULoadoutComponent::ApplyQuickBarEquipment(ABaseCharacter* Character)
{
	UInventoryManagerComponent* InventoryManagerComponent = GetController<AController>()->FindComponentByClass<UInventoryManagerComponent>();
	if (!InventoryManagerComponent)
	{
		return;
	}
	
	UQuickBarComponent* QuickBarComponent = Character->FindComponentByClass<UQuickBarComponent>();
	if (!QuickBarComponent)
	{
		return;
	}
	
	for	(const auto& Item : CurrentLoadout.ItemsToQuickBar)
	{
		if (QuickBarComponent->IsValidSlotIndex(Item.Key))
		{
			if (UInventoryItemInstance* ItemInst = InventoryManagerComponent->AddItemDefinition(Item.Value))
			{
				QuickBarComponent->TryAddItemToSlot(ItemInst,Item.Key);
			}
		}
	}
}

void ULoadoutComponent::ApplyEquipment(ABaseCharacter* Character)
{
	UEquipmentManagerComponent* EquipmentManagerComponent = Character->FindComponentByClass<UEquipmentManagerComponent>();
	if (!EquipmentManagerComponent)
	{
		return;
	}

	for (const TSubclassOf<UEquipmentDefinition>& ItemToEquip : CurrentLoadout.ItemsToEquip)
	{
		EquipmentManagerComponent->EquipItem(ItemToEquip);
	}
}

void ULoadoutComponent::ApplyAbilities(ABaseCharacter* Character)
{
	Character->TryApplyAbilitySet(CurrentLoadout.AbilityToGrant);
}
