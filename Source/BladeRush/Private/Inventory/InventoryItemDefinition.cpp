// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryItemDefinition.h"

UInventoryItemDefinition::UInventoryItemDefinition(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

const UInventoryItemFragment* UInventoryItemDefinition::FindFragmentByClass(
	TSubclassOf<UInventoryItemFragment> FragmentClass) const
{
	for (const UInventoryItemFragment* Fragment : Fragments)
	{
		if (Fragment && Fragment->IsA(FragmentClass))
		{
			return Fragment;
		}
	}
	return nullptr;
}
