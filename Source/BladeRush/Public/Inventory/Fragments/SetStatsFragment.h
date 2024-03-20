// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItemInfoFragment.h"
#include "UObject/Object.h"
#include "SetStatsFragment.generated.h"

struct FGameplayTag;

UCLASS()
class USetStatsFragment : public UInventoryItemFragment
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category="Equipment")
	TMap<FGameplayTag, int32> InitialItemStats;

public:
	virtual void OnInstanceCreated(UInventoryItemInstance* Instance) const override;
	
	int32 GetItemStatByTag(FGameplayTag Tag) const;
};