// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "Fragments/InventoryItemInfoFragment.h"
#include "QuickBarComponent.generated.h"

class UInventoryItemInstance;
class UEquipmentInstance;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FActiveSlotIndexChanged,int32, NewActiveSlotIndex,const UInventoryItemInstance*,ItemInstance);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSlotsArrayChanged,TArray<UInventoryItemInstance*>,Slots);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemAddedToSlot,int32, NewActiveSlotIndex, FInventoryItemInfo, InventoryItemInfo);

/**
 * Quick bar component class
 */
UCLASS(Blueprintable,BlueprintType, meta=(BlueprintSpawnableComponent))
class BLADERUSH_API UQuickBarComponent : public UPawnComponent
{
	GENERATED_BODY()
public:
	UQuickBarComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	bool TryAddItemToSlot(UInventoryItemInstance* InventoryItemInstance,int32 SlotIndex,bool bMakeThisSlotActive = true);

	UFUNCTION(BlueprintCallable)
	void SetActiveSlotIndex(int32 NewIndex);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	UInventoryItemInstance* RemoveItemFromSlot(int32 SlotIndex);

	FActiveSlotIndexChanged OnActiveSlotIndexChanged;
	FSlotsArrayChanged OnSlotsArrayChanged;
	FOnItemAddedToSlot OnItemAddedToSlot;
	
	UFUNCTION(BlueprintCallable)
	int32 GetMaxSlotsCount() const {return MaxSlotsCount;}
	
protected:
	virtual void BeginPlay() override;

	void EquipItemInActiveSlot(int32 NewActiveSlotIndex);
	void UnequipItemInActiveSlot();
	
private:
	UFUNCTION(Server,Reliable)
	void Server_SetActiveSlotIndex(const int32& NewIndex);

	UFUNCTION(Client,Reliable)
	void Clinet_OnInventoryItemAddedToSlot(int32 NewActiveSlotIndex, const FInventoryItemInfo& InventoryItemInfo);
	
	UPROPERTY(ReplicatedUsing=OnRep_Slots)
	TArray<TObjectPtr<UInventoryItemInstance>> Slots;

	UPROPERTY()
	TObjectPtr<UEquipmentInstance> EquippedItem;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(AllowPrivateAccess))
	int32 MaxSlotsCount = 3;

	UPROPERTY(ReplicatedUsing=OnRep_ActiveSlotIndex)
	int32 ActiveSlotIndex = -1;

	UFUNCTION()
	void OnRep_ActiveSlotIndex();

	UFUNCTION()
	void OnRep_Slots();
};
