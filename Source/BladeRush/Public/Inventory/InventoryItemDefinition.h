// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItemInstance.h"
#include "InventoryItemDefinition.generated.h"

UCLASS()
class UInventoryItemFragment : public UObject
{
	GENERATED_BODY()

public:
	virtual void OnInstanceCreated(UInventoryItemInstance* Instance) const {}
};

/**
 * Inventory Item definition class
 */
UCLASS()
class BLADERUSH_API UInventoryItemDefinition : public UObject
{
	GENERATED_BODY()
	
public:
	UInventoryItemDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Name")
	FText DisplayName;

	//TODO: TSet ?
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Fragments")
	TArray<UInventoryItemFragment*> Fragments;

	const UInventoryItemFragment* FindFragmentByClass(TSubclassOf<UInventoryItemFragment> FragmentClass) const;
};
