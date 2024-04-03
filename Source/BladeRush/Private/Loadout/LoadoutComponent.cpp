// Fill out your copyright notice in the Description page of Project Settings.


#include "Loadout/LoadoutComponent.h"

#include "Characters/BaseCharacter.h"
#include "Data/AvailableLoadoutDataAsset.h"
#include "Equipment/EquipmentDefinition.h"
#include "Equipment/EquipmentManagerComponent.h"
#include "Inventory/InventoryManagerComponent.h"
#include "Inventory/QuickBarComponent.h"
#include "Inventory/Fragments/EquippableItemFragment.h"
#include "Loadout/LoadoutAbilitiesDefinition.h"
#include "Loadout/LoadoutItemDefinition.h"
#include "Loadout/LoadoutItemQuickBarDef.h"
#include "Net/UnrealNetwork.h"


ULoadoutComponent::ULoadoutComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void ULoadoutComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass,CurrentLoadout);
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

void ULoadoutComponent::TrySetCurrentLoadout(const FLoadoutDefinitions& LoadoutDefinitions)
{
	if (HasAuthority())
	{
		SetCurrentLoadoutFromDef(LoadoutDefinitions);
	}
	else
	{
		SetCurrentLoadoutFromDef_Server(LoadoutDefinitions);
	}
}

void ULoadoutComponent::SetCurrentLoadoutFromDef_Server_Implementation(const FLoadoutDefinitions& LoadoutDefinitions)
{
	SetCurrentLoadoutFromDef(LoadoutDefinitions);
}

void ULoadoutComponent::SetCurrentLoadoutFromDef(const FLoadoutDefinitions& LoadoutDefinitions)
{
	if (!HasAuthority())
	{
		return;
	}

	FCharacterLoadout LoadoutToSet;
	
	for (const TSubclassOf<ULoadoutItemQuickBarDef> ItemQuickBarDef : LoadoutDefinitions.QuickBarDefinitions)
	{
		if (AvailableLoadoutDataAsset->ContainsQuickBarDef(ItemQuickBarDef))
		{
			LoadoutToSet.ItemsToQuickBar.Add(CreateLoadoutItem(ItemQuickBarDef));
		}
	}

	for (const TSubclassOf<ULoadoutItemDefinition> EquipmDef : LoadoutDefinitions.EquipmentDefinitions)
	{
		if (AvailableLoadoutDataAsset->ContainsEquipDef(EquipmDef))
		{
			LoadoutToSet.ItemsToQuickBar.Add(CreateLoadoutItem(EquipmDef));
		}
	}

	for (const TSubclassOf<ULoadoutAbilitiesDefinition> AbilitiesDef : LoadoutDefinitions.AbilityDefinitions)
	{
		if (AvailableLoadoutDataAsset->ContainsAbilitiesDef(AbilitiesDef))
		{
			LoadoutToSet.AbilitySetsToGrant.Add(CreateLoadoutAbilities(AbilitiesDef));
		}
	}

	SetCurrentLoadout(LoadoutToSet);
}

FLoadoutItem ULoadoutComponent::CreateLoadoutItem(TSubclassOf<ULoadoutItemDefinition> LoadoutItemDefClass)
{
	const ULoadoutItemDefinition* LoadoutItemDefinition = LoadoutItemDefClass.GetDefaultObject();

	TSubclassOf<UInventoryItemDefinition> ItemDefClass = LoadoutItemDefinition->GetItemDefinitionSoftClass().LoadSynchronous();
	
	FLoadoutItem LoadoutItemToEquip;
	LoadoutItemToEquip.LoadoutDefinition = LoadoutItemDefClass;
	LoadoutItemToEquip.ItemDef = ItemDefClass;
	

	return LoadoutItemToEquip;
}

FLoadoutAbilities ULoadoutComponent::CreateLoadoutAbilities(
	TSubclassOf<ULoadoutAbilitiesDefinition> LoadoutAbilitiesDefClass)
{
	const ULoadoutAbilitiesDefinition* AbilitiesDefinition = LoadoutAbilitiesDefClass.GetDefaultObject();

	UShooterAbilitySet* ShooterAbilitySet = AbilitiesDefinition->GetShooterAbilitySetSoft().LoadSynchronous();
	
	FLoadoutAbilities LoadoutItemToEquip;
	LoadoutItemToEquip.AbilitySet = ShooterAbilitySet;
	LoadoutItemToEquip.LoadoutDefinition = LoadoutAbilitiesDefClass;
	

	return LoadoutItemToEquip;
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
	
	for	(const FLoadoutItem& Item : CurrentLoadout.ItemsToQuickBar)
	{
		ULoadoutItemQuickBarDef* LoadoutItemQuickBarDef = Cast<ULoadoutItemQuickBarDef>(Item.LoadoutDefinition.GetDefaultObject());
		if (!LoadoutItemQuickBarDef)
		{
			continue;
		}
		
		if (QuickBarComponent->IsValidSlotIndex(LoadoutItemQuickBarDef->GetSlotIndex()))
		{
			if (UInventoryItemInstance* ItemInst = InventoryManagerComponent->AddItemDefinition(Item.ItemDef))
			{
				QuickBarComponent->TryAddItemToSlot(ItemInst,LoadoutItemQuickBarDef->GetSlotIndex());
			}
		}
	}
}

void ULoadoutComponent::ApplyEquipment(ABaseCharacter* Character)
{
	UInventoryManagerComponent* InventoryManagerComponent = GetController<AController>()->FindComponentByClass<UInventoryManagerComponent>();
	if (!InventoryManagerComponent)
	{
		return;
	}
	
	UEquipmentManagerComponent* EquipmentManagerComponent = Character->FindComponentByClass<UEquipmentManagerComponent>();
	if (!EquipmentManagerComponent)
	{
		return;
	}
	
	for (const FLoadoutItem& Item : CurrentLoadout.ItemsToEquip)
	{
		TryEquipItem(InventoryManagerComponent,EquipmentManagerComponent,Item.ItemDef);
	}
}

void ULoadoutComponent::ApplyAbilities(ABaseCharacter* Character)
{
	for (const FLoadoutAbilities& LoadoutAbilitySet : CurrentLoadout.AbilitySetsToGrant)
	{
		Character->TryApplyAbilitySet(LoadoutAbilitySet.AbilitySet,true);
	}
}



void ULoadoutComponent::TryEquipItem(UInventoryManagerComponent* InventoryComp, UEquipmentManagerComponent* EquipmentComp,TSubclassOf<UInventoryItemDefinition> ItemDef)
{
	if (!InventoryComp || !EquipmentComp)
	{
		return;
	}
	
	const UInventoryItemInstance* ItemInst = InventoryComp->AddItemDefinition(ItemDef);
	if (!ItemInst)
	{
		return;
	}

	const UEquippableItemFragment* EquippableItemFragment = ItemInst->FindFragmentByClass<UEquippableItemFragment>();
	if (!EquippableItemFragment)
	{
		return;
	}

	if (EquippableItemFragment->EquipmentDefinitionClass)
	{
		EquipmentComp->EquipItem(EquippableItemFragment->EquipmentDefinitionClass);
	}
}
