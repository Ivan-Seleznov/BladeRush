// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Data/GameplayTagStack.h"
#include "InventoryItemInstance.generated.h"

class UInventoryItemDefinition;
class UInventoryItemFragment;

UENUM()
enum ESlotType
{
	None,
	Weapon
};
/**
 * InventoryItemInstance class
 */
UCLASS(BlueprintType)
class BLADERUSH_API UInventoryItemInstance : public UObject
{
	GENERATED_BODY()
public:
	UInventoryItemInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual bool IsSupportedForNetworking() const override { return true; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, BlueprintPure=false, meta=(DeterminesOutputType=FragmentClass))
	const UInventoryItemFragment* FindFragmentByClass(TSubclassOf<UInventoryItemFragment> FragmentClass) const;

	template <typename ResultClass>
	const ResultClass* FindFragmentByClass() const
	{
		return (ResultClass*)FindFragmentByClass(ResultClass::StaticClass());
	}
	
	void SetItemDefClass(TSubclassOf<UInventoryItemDefinition> InDef);

	UFUNCTION(BlueprintCallable)
	ESlotType GetSlotType() {return SlotType;}

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	void AddStatTagStack(FGameplayTag Tag, int32 StackCount);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category= Inventory)
	void RemoveStatTagStack(FGameplayTag Tag, int32 StackCount);

	UFUNCTION(BlueprintCallable, Category=Inventory)
	int32 GetStatTagStackCount(FGameplayTag Tag) const;
	
	UFUNCTION(BlueprintCallable, Category=Inventory)
	bool HasStatTag(FGameplayTag Tag) const;
	void SetItemDef(TSubclassOf<UInventoryItemDefinition> InDef);

private:
	UPROPERTY(Replicated)
	TEnumAsByte<ESlotType> SlotType = ESlotType::None;
	
	UPROPERTY(Replicated)
	TSubclassOf<UInventoryItemDefinition> InventoryItemDefinitionClass;
	
	UPROPERTY(Replicated)
	FGameplayTagStackContainer StatTags;
};
