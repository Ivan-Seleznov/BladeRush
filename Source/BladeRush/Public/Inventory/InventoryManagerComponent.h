// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItemInstance.h" // TODO: remove enum from inventory item instance
#include "Components/ActorComponent.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "InventoryManagerComponent.generated.h"

class UInventoryItemDefinition;
class UInventoryItemInstance;
class UInventoryManagerComponent;
class UObject;
struct FFrame;
struct FInventoryList;
struct FNetDeltaSerializeInfo;
struct FReplicationFlags;

USTRUCT(BlueprintType)
struct FInventoryEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FInventoryEntry() {  }

	UPROPERTY()
	UInventoryItemInstance* Instance = nullptr;

	UPROPERTY()
	int32 StackCount = 0;

	UPROPERTY(NotReplicated)
	int32 LastObservedCount = INDEX_NONE;
private:
	friend FInventoryList;
	friend UInventoryManagerComponent;
};

USTRUCT(BlueprintType)
struct FInventoryList : public FFastArraySerializer
{
	GENERATED_BODY()
	
	FInventoryList()
	: OwnerComponent(nullptr)
	{
	}

	FInventoryList(UActorComponent* InOwnerComponent)
		: OwnerComponent(InOwnerComponent)
	{
	}

	//~FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer contract

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FInventoryEntry, FInventoryList>(Entries, DeltaParms, *this);
	}
	
	UInventoryItemInstance* AddEntry(TSubclassOf<UInventoryItemDefinition> ItemDefinitionClass,int32 StackCount);
	
	TArray<UInventoryItemInstance*> GetAllItemInstances() const;
	
	void RemoveEntry(UInventoryItemInstance* Instance);

	TArray<UInventoryItemInstance*> GetAllItemsInSlot(const ESlotType& SlotType) const;
	
private:
	friend UInventoryManagerComponent;
	
	UPROPERTY()
	TArray<FInventoryEntry> Entries;

	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;
};

template<>
struct TStructOpsTypeTraits<FInventoryList> : public TStructOpsTypeTraitsBase2<FInventoryList>
{
	enum { WithNetDeltaSerializer = true };
};

UCLASS(BlueprintType)
class BLADERUSH_API UInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	UInventoryItemInstance* AddItemDefinition(TSubclassOf<UInventoryItemDefinition> ItemDef, int32 StackCount = 1);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	void RemoveItemInstance(UInventoryItemInstance* ItemInstance);

	UFUNCTION(BlueprintCallable, Category=Inventory)
	TArray<UInventoryItemInstance*> GetAllItemInstances(); 

	UFUNCTION(BlueprintCallable, Category=Inventory)
	TArray<UInventoryItemInstance*> GetAllItemsInSlot(const ESlotType& SlotType) const;

	
	//~UObject interface
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	virtual void ReadyForReplication() override;
	//~End of UObject interface
	
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

private:
	UPROPERTY(Replicated)
	FInventoryList InventoryList;
};