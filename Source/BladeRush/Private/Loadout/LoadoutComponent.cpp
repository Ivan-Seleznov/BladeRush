// Fill out your copyright notice in the Description page of Project Settings.


#include "Loadout/LoadoutComponent.h"

#include "Characters/BaseCharacter.h"
#include "Data/AvailableLoadoutDataAsset.h"
#include "Equipment/EquipmentDefinition.h"
#include "Equipment/EquipmentInstance.h"
#include "Equipment/EquipmentManagerComponent.h"
#include "Inventory/InventoryManagerComponent.h"
#include "Inventory/QuickBarComponent.h"
#include "Inventory/Fragments/EquippableItemFragment.h"
#include "Loadout/LoadoutAbilitiesDefinition.h"
#include "Loadout/LoadoutEquipmentDefinition.h"
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
	ClearPrevLoadout();
	
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
			LoadoutToSet.ItemsToQuickBar.Add(CreateLoadoutQuickBarItem(ItemQuickBarDef));
		}
	}

	for (const TSubclassOf<ULoadoutEquipmentDefinition> EquipmDef : LoadoutDefinitions.EquipmentDefinitions)
	{
		if (AvailableLoadoutDataAsset->ContainsEquipDef(EquipmDef))
		{
			LoadoutToSet.ItemsToEquip.Add(CreateLoadoutEquipmentItem(EquipmDef));
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

FLoadoutEquipment ULoadoutComponent::CreateLoadoutEquipmentItem(
	TSubclassOf<ULoadoutEquipmentDefinition> LoadoutEquipmentDefClass)
{
	const ULoadoutEquipmentDefinition* LoadoutEquipmentDefinition = LoadoutEquipmentDefClass.GetDefaultObject();

	TSubclassOf<ULoadoutEquipmentDefinition> EquipmentDefClass = LoadoutEquipmentDefinition->GetEquipmentDefinitionSoftClass().LoadSynchronous();

	FLoadoutEquipment LoadoutItemToEquip;
	LoadoutItemToEquip.LoadoutDefinition = LoadoutEquipmentDefClass;
	LoadoutItemToEquip.EquipmentDef = EquipmentDefClass;

	return LoadoutItemToEquip;
}

FLoadoutQuickBarItem ULoadoutComponent::CreateLoadoutQuickBarItem(TSubclassOf<ULoadoutItemQuickBarDef> LoadoutItemDefClass)
{
	const ULoadoutItemQuickBarDef* LoadoutItemDefinition = LoadoutItemDefClass.GetDefaultObject();

	TSubclassOf<UInventoryItemDefinition> ItemDefClass = LoadoutItemDefinition->GetItemDefinitionSoftClass().LoadSynchronous();
	
	FLoadoutQuickBarItem LoadoutItemToQuickBar;
	LoadoutItemToQuickBar.LoadoutDefinition = LoadoutItemDefClass;
	LoadoutItemToQuickBar.ItemDef = ItemDefClass;

	return LoadoutItemToQuickBar;
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
	
	for	(const FLoadoutQuickBarItem& Item : CurrentLoadout.ItemsToQuickBar)
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
	UEquipmentManagerComponent* EquipmentManagerComponent = Character->FindComponentByClass<UEquipmentManagerComponent>();
	if (!EquipmentManagerComponent)
	{
		return;
	}
	
	for (const FLoadoutEquipment& Item : CurrentLoadout.ItemsToEquip)
	{
		if (UEquipmentInstance* EquipmentInstance = EquipmentManagerComponent->EquipItem(Item.EquipmentDef))
		{
			CurrentEquipmentInstances.Add(EquipmentInstance);
		}
	}
}

void ULoadoutComponent::ApplyAbilities(ABaseCharacter* Character)
{
	UPlayerAbilitySystemComponent* AbilitySystemComponent = Character->GetCharacterAbilitySystemComponent();
	if (!AbilitySystemComponent)
	{
		return;
	}

	for (const FLoadoutAbilities& LoadoutAbilitySet : CurrentLoadout.AbilitySetsToGrant)
	{
		if (LoadoutAbilitySet.AbilitySet)
		{
			LoadoutAbilitySet.AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, &CurrentLoadoutGrantedHandles);
		}
	}
	
}

void ULoadoutComponent::ClearPrevLoadout()
{
	APlayerController* PC = GetControllerChecked<APlayerController>();

	/*Unequip prev items*/
	UEquipmentManagerComponent* EquipmentManagerComponent = PC->FindComponentByClass<UEquipmentManagerComponent>();
	if (!EquipmentManagerComponent)
	{
		return;
	}

	for (UEquipmentInstance* CurrentEquipmentInstance : CurrentEquipmentInstances)
	{
		EquipmentManagerComponent->UnequipItem(CurrentEquipmentInstance);
	}

	/*Remove prev ability sets*/
	const ABaseCharacter* Character = GetPawn<ABaseCharacter>();
	if (!Character)
	{
		return;
	}
	
	UPlayerAbilitySystemComponent* AbilitySystemComponent = Character->GetCharacterAbilitySystemComponent();
	if (!AbilitySystemComponent)
	{
		return;
	}

	CurrentLoadoutGrantedHandles.TakeFromAbilitySystem(AbilitySystemComponent);
}
