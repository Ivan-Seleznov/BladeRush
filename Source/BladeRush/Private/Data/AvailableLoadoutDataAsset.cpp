// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/AvailableLoadoutDataAsset.h"

bool UAvailableLoadoutDataAsset::ContainsQuickBarDef(TSubclassOf<ULoadoutItemQuickBarDef> QuickBarDef) const
{
	return LoadoutDefinitions.QuickBarDefinitions.Contains(QuickBarDef);
}

bool UAvailableLoadoutDataAsset::ContainsEquipDef(TSubclassOf<ULoadoutItemDefinition> ItemDef) const
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

const TSubclassOf<ULoadoutItemDefinition>* UAvailableLoadoutDataAsset::FindEquipDefByName(FName InName) const
{
	return LoadoutDefinitions.EquipmentDefinitions.FindByPredicate(
	[&InName](TSubclassOf<ULoadoutItemDefinition> ItemDef)
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
