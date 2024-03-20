// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "GAS/ShooterAbilitySet.h"
#include "EquipmentManagerComponent.generated.h"

class UEquipmentDefinition;
class UEquipmentInstance;
struct FEquipmentList;

USTRUCT(BlueprintType)
struct FAppliedEquipmentEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()
	
	FAppliedEquipmentEntry() {  }
	
	UPROPERTY()
	TSubclassOf<UEquipmentDefinition> EquipmentDefinition;

	UPROPERTY()
	UEquipmentInstance* Instance = nullptr;
	
	UPROPERTY(NotReplicated)
	FAbilitySet_GrantedHandles GrantedHandles;
private:
	friend FEquipmentList;
};

USTRUCT(BlueprintType)
struct FEquipmentList : public FFastArraySerializer
{
	GENERATED_BODY()

	FEquipmentList()
	: OwnerComponent(nullptr)
	{
	}

	FEquipmentList(UActorComponent* InOwnerComponent)
		: OwnerComponent(InOwnerComponent)
	{
	}
	
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FAppliedEquipmentEntry, FEquipmentList>(Entries, DeltaParms, *this);
	}
	
	UEquipmentInstance* AddEntry(TSubclassOf<UEquipmentDefinition> EquipmentDefinition);
	void RemoveEntry(UEquipmentInstance* Instance);

	TArray<UEquipmentInstance*> GetAllEquipmentInstances() const;

private:
	friend UEquipmentManagerComponent;

	UPlayerAbilitySystemComponent* GetAbilitySystemComponent() const;

	UPROPERTY()
	TArray<FAppliedEquipmentEntry> Entries;
	
	UPROPERTY(NotReplicated)
	UActorComponent* OwnerComponent;
};

template<>
struct TStructOpsTypeTraits<FEquipmentList> : public TStructOpsTypeTraitsBase2<FEquipmentList>
{
	enum { WithNetDeltaSerializer = true };
};

/**
 * 
 */
UCLASS(BlueprintType)
class BLADERUSH_API UEquipmentManagerComponent : public UPawnComponent
{
	GENERATED_BODY()
public:
	UEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	UEquipmentInstance* EquipItem(TSubclassOf<UEquipmentDefinition> EquipmentDefinition);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void UnequipItem(UEquipmentInstance* ItemInstance);

	//~UObject interface
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	//~End of UObject interface

	//~UActorComponent interface
	//virtual void EndPlay() override;
	virtual void UninitializeComponent() override;
	virtual void ReadyForReplication() override;
	//~End of UActorComponent interface

	/** Returns the first equipped instance of a given type, or nullptr if none are found */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UEquipmentInstance* GetFirstInstanceOfType(TSubclassOf<UEquipmentInstance> InstanceType);

	/** Returns all equipped instances of a given type, or an empty array if none are found */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<UEquipmentInstance*> GetEquipmentInstancesOfType(TSubclassOf<UEquipmentInstance> InstanceType) const;

	template<typename T>
	T* GetFirstInstanceOfType();

	template<typename T>
	TArray<T*> GetEquipmentInstancesOfType();
private:
	UPROPERTY(Replicated)
	FEquipmentList EquipmentList;
};

template <typename T>
T* UEquipmentManagerComponent::GetFirstInstanceOfType()
{
	return (T*)GetFirstInstanceOfType(T::StaticClass());
}

template <typename T>
TArray<T*> UEquipmentManagerComponent::GetEquipmentInstancesOfType()
{
	TArray<T*> Results;
	for (const FAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		if (T* Instance = Cast<T>(Entry.Instance))
		{
			Results.Add(Instance);
		}
	}
	return Results;
}
