// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryItemInstance.h"

#include "Inventory/InventoryItemDefinition.h"
#include "Net/UnrealNetwork.h"

UInventoryItemInstance::UInventoryItemInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UInventoryItemInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, InventoryItemDefinitionClass);
	DOREPLIFETIME(ThisClass,SlotType);
	DOREPLIFETIME(ThisClass, StatTags);
}

const UInventoryItemFragment* UInventoryItemInstance::FindFragmentByClass(
	TSubclassOf<UInventoryItemFragment> FragmentClass) const
{
	if (InventoryItemDefinitionClass && FragmentClass)
	{
		return InventoryItemDefinitionClass->GetDefaultObject<UInventoryItemDefinition>()->FindFragmentByClass(FragmentClass);
	}
	
	return nullptr;
}

void UInventoryItemInstance::SetItemDefClass(TSubclassOf<UInventoryItemDefinition> InDef)
{
	InventoryItemDefinitionClass = InDef;
}

void UInventoryItemInstance::AddStatTagStack(FGameplayTag Tag, int32 StackCount)
{
	StatTags.AddStack(Tag, StackCount);
}

void UInventoryItemInstance::RemoveStatTagStack(FGameplayTag Tag, int32 StackCount)
{
	StatTags.RemoveStack(Tag, StackCount);
}

int32 UInventoryItemInstance::GetStatTagStackCount(FGameplayTag Tag) const
{
	return StatTags.GetStackCount(Tag);
}

bool UInventoryItemInstance::HasStatTag(FGameplayTag Tag) const
{
	return StatTags.ContainsTag(Tag);
}

void UInventoryItemInstance::SetItemDef(TSubclassOf<UInventoryItemDefinition> InDef)
{
	InventoryItemDefinitionClass = InDef;
}
