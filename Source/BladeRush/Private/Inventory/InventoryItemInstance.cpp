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
