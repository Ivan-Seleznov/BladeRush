// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/AvailableLoadoutDataAsset.h"
#include "Loadout/LoadoutAbilitiesDefinition.h"
#include "Loadout/LoadoutItemQuickBarDef.h"
#include "Loadout/LoadoutEquipmentDefinition.h"

bool UAvailableLoadoutDataAsset::ContainsQuickBarDef(TSubclassOf<ULoadoutItemQuickBarDef> QuickBarDef) const
{
	return LoadoutDefinitions.QuickBarDefinitions.Contains(QuickBarDef);
}

bool UAvailableLoadoutDataAsset::ContainsEquipDef(TSubclassOf<ULoadoutEquipmentDefinition> ItemDef) const
{
	return LoadoutDefinitions.EquipmentDefinitions.Contains(ItemDef);
}


bool UAvailableLoadoutDataAsset::ContainsAbilitiesDef(TSubclassOf<ULoadoutAbilitiesDefinition> AbilitiesDef) const
{
	return LoadoutDefinitions.AbilityDefinitions.Contains(AbilitiesDef);
}

const TSubclassOf<ULoadoutItemQuickBarDef>* UAvailableLoadoutDataAsset::FindQuickBarDefByName(FName InName) const
{
	return LoadoutDefinitions.QuickBarDefinitions.FindByPredicate(
		[&InName](TSubclassOf<ULoadoutItemQuickBarDef> QuickBarDef)
		{
			return QuickBarDef.GetDefaultObject()->GetDisplayName() == InName;
		});
}

const TSubclassOf<ULoadoutEquipmentDefinition>* UAvailableLoadoutDataAsset::FindEquipDefByName(FName InName) const
{
	return LoadoutDefinitions.EquipmentDefinitions.FindByPredicate(
	[&InName](TSubclassOf<ULoadoutEquipmentDefinition> ItemDef)
		{
			return ItemDef.GetDefaultObject()->GetDisplayName() == InName;
		});
}

const TSubclassOf<ULoadoutAbilitiesDefinition>* UAvailableLoadoutDataAsset::FindAbilitiesDefByName(FName InName) const
{
	return LoadoutDefinitions.AbilityDefinitions.FindByPredicate(
	[&InName](TSubclassOf<ULoadoutAbilitiesDefinition> AbilitiesDef)
		{
			return AbilitiesDef.GetDefaultObject()->GetDisplayName() == InName;
		});
}
