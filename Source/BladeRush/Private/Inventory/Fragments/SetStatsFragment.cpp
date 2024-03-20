// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Fragments/SetStatsFragment.h"
#include "GameplayTagContainer.h"

void USetStatsFragment::OnInstanceCreated(UInventoryItemInstance* Instance) const
{
	for (const auto& KVP : InitialItemStats)
	{
		Instance->AddStatTagStack(KVP.Key, KVP.Value);
	}
}

int32 USetStatsFragment::GetItemStatByTag(FGameplayTag Tag) const
{
	if (const int32* StatPtr = InitialItemStats.Find(Tag))
	{
		return *StatPtr;
	}

	return 0;
}
