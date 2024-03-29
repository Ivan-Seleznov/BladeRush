// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/QuickBarComponent.h"

#include "Equipment/EquipmentDefinition.h"
#include "Equipment/EquipmentInstance.h"
#include "Equipment/EquipmentManagerComponent.h"
#include "Inventory/InventoryItemInstance.h"
#include "Inventory/Fragments/EquippableItemFragment.h"
#include "Inventory/Fragments/InventoryItemInfoFragment.h"
#include "Net/UnrealNetwork.h"

UQuickBarComponent::UQuickBarComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);
}

void UQuickBarComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Slots);
	DOREPLIFETIME(ThisClass, ActiveSlotIndex);
}

bool UQuickBarComponent::TryAddItemToSlot(UInventoryItemInstance* InventoryItemInstance, int32 SlotIndex,bool bMakeThisSlotActive)
{
	if (!Slots.IsValidIndex(SlotIndex) || !InventoryItemInstance || !GetOwner()->HasAuthority())
	{
		return false;
	}
	
	Slots[SlotIndex] = InventoryItemInstance;

	if (const UInventoryItemInfoFragment* InventoryItemFragment = InventoryItemInstance->FindFragmentByClass<UInventoryItemInfoFragment>())
	{
		OnItemAddedToSlot.Broadcast(SlotIndex,InventoryItemFragment->InventoryItemInfo);
		Clinet_OnInventoryItemAddedToSlot(SlotIndex,InventoryItemFragment->InventoryItemInfo);
	}
	
	
	if (bMakeThisSlotActive)
	{
		SetActiveIndex(SlotIndex);
	}

	return true;
}



void UQuickBarComponent::SetActiveSlotIndex(int32 NewIndex)
{
	if (ActiveSlotIndex > MaxSlotsCount)
	{
		return;
	}
	
	APawn* OwnerPawn = GetPawn<APawn>();
	if (!OwnerPawn)
	{
		return;
	}
	
	if (OwnerPawn->IsLocallyControlled())
	{
		Server_SetActiveSlotIndex(NewIndex);
	}
	else if (OwnerPawn->HasAuthority())
	{
		SetActiveIndex(NewIndex);
	}
}

void UQuickBarComponent::Server_SetActiveSlotIndex_Implementation(int32 NewIndex)
{
	SetActiveIndex(NewIndex);
}

UInventoryItemInstance* UQuickBarComponent::RemoveItemFromSlot(int32 SlotIndex)
{
	if (!GetOwner() || !GetOwner()->HasAuthority()) return nullptr;

	if (ActiveSlotIndex == SlotIndex)
	{
		UnequipItemInActiveSlot();
		ActiveSlotIndex = -1;
	}

	UInventoryItemInstance* Result = nullptr;
	if (Slots.IsValidIndex(ActiveSlotIndex))
	{
		Result = Slots[SlotIndex];
		Slots[SlotIndex] = nullptr;
	}
	
	return Result;
}

UInventoryItemInstance* UQuickBarComponent::GetCurrentInventoryItemInstance() const
{
	if (ActiveSlotIndex >= 0 && ActiveSlotIndex < Slots.Num())
	{
		return Slots[ActiveSlotIndex];
	}

	return nullptr;
}

void UQuickBarComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (Slots.Num() < MaxSlotsCount)
	{
		Slots.AddDefaulted(MaxSlotsCount - Slots.Num());
	}
}

void UQuickBarComponent::EquipItemInActiveSlot(int32 NewActiveSlotIndex)
{
	APawn* OwnerPawn = GetPawn<APawn>();
	if (!OwnerPawn || !OwnerPawn->HasAuthority()) return;
	
	if (!Slots.IsValidIndex(NewActiveSlotIndex)) return;

	UInventoryItemInstance* SlotItemInstance = Slots[NewActiveSlotIndex];
	if (!SlotItemInstance) return;

	const UEquippableItemFragment* EquippableItemFragment = SlotItemInstance->FindFragmentByClass<UEquippableItemFragment>();
	if (!EquippableItemFragment) return;

	TSubclassOf<UEquipmentDefinition> EquipDefClass = EquippableItemFragment->EquipmentDefinitionClass;
	if (!EquipDefClass) return;

	UEquipmentManagerComponent* EquipmentManagerComponent = OwnerPawn->FindComponentByClass<UEquipmentManagerComponent>();
	if (!EquipmentManagerComponent) return;

	EquippedItem = EquipmentManagerComponent->EquipItem(EquipDefClass);
	if (EquippedItem)
	{
		EquippedItem->SetInstigator(SlotItemInstance);
	}
	ActiveSlotIndex = NewActiveSlotIndex;
	OnRep_ActiveSlotIndex();
}

void UQuickBarComponent::UnequipItemInActiveSlot()
{
	APawn* OwnerPawn = GetPawn<APawn>();
	if (!OwnerPawn || !OwnerPawn->HasAuthority() || !EquippedItem) return;
	
	UEquipmentManagerComponent* EquipmentManagerComponent = OwnerPawn->FindComponentByClass<UEquipmentManagerComponent>();
	if (!EquipmentManagerComponent) return;
	
	EquipmentManagerComponent->UnequipItem(EquippedItem);
	EquippedItem = nullptr;
	
}

void UQuickBarComponent::SetActiveIndex(int32 NewIndex)
{
	if (ActiveSlotIndex > MaxSlotsCount)
	{
		return;
	}

	if (NewIndex < 0)
	{
		UnequipItemInActiveSlot();
		
		ActiveSlotIndex = -1;
		OnRep_ActiveSlotIndex();
		return;	
	}
	
	if (Slots.IsValidIndex(NewIndex) && Slots[NewIndex])
	{
		UnequipItemInActiveSlot();

		EquipItemInActiveSlot(NewIndex);
	}
}

void UQuickBarComponent::Clinet_OnInventoryItemAddedToSlot_Implementation(int32 NewActiveSlotIndex,
                                                                          const FInventoryItemInfo& InventoryItemInfo)
{
	OnItemAddedToSlot.Broadcast(NewActiveSlotIndex,InventoryItemInfo);
}

void UQuickBarComponent::OnRep_ActiveSlotIndex()
{
	UInventoryItemInstance* ActiveInvItemInstance = nullptr;
	if (Slots.IsValidIndex(ActiveSlotIndex))
	{
		ActiveInvItemInstance = Slots[ActiveSlotIndex];
	}	
	OnActiveSlotIndexChanged.Broadcast(ActiveSlotIndex,ActiveInvItemInstance);
}

void UQuickBarComponent::OnRep_Slots()
{
	OnSlotsArrayChanged.Broadcast(Slots);
}
