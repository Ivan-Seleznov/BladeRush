// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryManagerComponent.h"

#include "Engine/ActorChannel.h"
#include "Inventory/InventoryItemDefinition.h"
#include "Net/UnrealNetwork.h"


//~FFastArraySerializer contract
void FInventoryList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	for (int32 Index : RemovedIndices)
	{
		FInventoryEntry& Stack = Entries[Index];
		Stack.LastObservedCount = Stack.StackCount;
	}
}

void FInventoryList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (int32 Index : AddedIndices)
	{
		FInventoryEntry& Stack = Entries[Index];
		Stack.LastObservedCount = Stack.StackCount;
	}
}

void FInventoryList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{	
	for (int32 Index : ChangedIndices)
	{
		FInventoryEntry& Stack = Entries[Index];
		check(Stack.LastObservedCount != INDEX_NONE);
		Stack.LastObservedCount = Stack.StackCount;
	}
}


//~End of FFastArraySerializer contract


UInventoryItemInstance* FInventoryList::AddEntry(TSubclassOf<UInventoryItemDefinition> ItemDefinitionClass,
	int32 StackCount)
{
	if (!OwnerComponent || !ItemDefinitionClass) return nullptr;

	AActor* OwningActor = OwnerComponent->GetOwner();
	if (!OwningActor->HasAuthority()) return nullptr;
	
	FInventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Instance = NewObject<UInventoryItemInstance>(OwningActor); //TODO: Actor in UObject ctor?

	NewEntry.Instance->SetItemDefClass(ItemDefinitionClass);
	for (const UInventoryItemFragment* InventoryItemFragment : GetDefault<UInventoryItemDefinition>(ItemDefinitionClass)->Fragments)
	{
		InventoryItemFragment->OnInstanceCreated(NewEntry.Instance);
	}

	NewEntry.StackCount = StackCount;
	
	MarkItemDirty(NewEntry);
	
	return NewEntry.Instance;
}

TArray<UInventoryItemInstance*> FInventoryList::GetAllItemInstances() const
{
	TArray<UInventoryItemInstance*> InventoryItemInstances;
	InventoryItemInstances.Reserve(Entries.Num());
	for (const FInventoryEntry& Entry : Entries)
	{
		if (Entry.Instance)
		{
			InventoryItemInstances.Add(Entry.Instance);
		}
	}

	return InventoryItemInstances;
}

void FInventoryList::RemoveEntry(UInventoryItemInstance* Instance)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FInventoryEntry& Entry = *EntryIt;
		if (Entry.Instance == Instance)
		{
			EntryIt.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}

TArray<UInventoryItemInstance*> FInventoryList::GetAllItemsInSlot(const ESlotType& SlotType) const
{
	TArray<UInventoryItemInstance*> Result;
	for (const FInventoryEntry& Entry : Entries)
	{
		if (Entry.Instance->GetSlotType() == SlotType)
		{
			Result.Add(Entry.Instance);
		}
	}

	return Result;
}

///////////////////////////
//InventoryManagerComponent

UInventoryManagerComponent::UInventoryManagerComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);
	InventoryList.OwnerComponent = this;
}

UInventoryItemInstance* UInventoryManagerComponent::AddItemDefinition(TSubclassOf<UInventoryItemDefinition> ItemDef,
                                                                      int32 StackCount)
{
	if (!ItemDef) return nullptr;
	
	UInventoryItemInstance* Result = InventoryList.AddEntry(ItemDef, StackCount);
	if (!Result) return nullptr;

	
	if (IsUsingRegisteredSubObjectList() && IsReadyForReplication())
	{
		AddReplicatedSubObject(Result);
	}
	
	return Result;
}

void UInventoryManagerComponent::RemoveItemInstance(UInventoryItemInstance* ItemInstance)
{
	InventoryList.RemoveEntry(ItemInstance);

	if (ItemInstance && IsUsingRegisteredSubObjectList())
	{
		RemoveReplicatedSubObject(ItemInstance);
	}
}

TArray<UInventoryItemInstance*> UInventoryManagerComponent::GetAllItemInstances()
{
	return InventoryList.GetAllItemInstances();
}

TArray<UInventoryItemInstance*> UInventoryManagerComponent::GetAllItemsInSlot(const ESlotType& SlotType) const
{
	return InventoryList.GetAllItemsInSlot(SlotType);
}

//~UObject interface
bool UInventoryManagerComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch,
	FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (FInventoryEntry& Entry : InventoryList.Entries)
	{
		UInventoryItemInstance* Instance = Entry.Instance;

		if (Instance && IsValid(Instance))
		{
			WroteSomething |= Channel->ReplicateSubobject(Instance, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;}

void UInventoryManagerComponent::ReadyForReplication()
{
	Super::ReadyForReplication();

	// Register existing InventoryItemInstance
	if (IsUsingRegisteredSubObjectList())
	{
		for (const FInventoryEntry& Entry : InventoryList.Entries)
		{
			UInventoryItemInstance* Instance = Entry.Instance;

			if (IsValid(Instance))
			{
				AddReplicatedSubObject(Instance);
			}
		}
	}
}
//~End of UObject interface

void UInventoryManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, InventoryList);
}

