// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventoryItemDefinition.h"
#include "InventoryItemInfoFragment.generated.h"

USTRUCT(BlueprintType)
struct FInventoryItemInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	UTexture2D* ImageTexture;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FText ItemNameText;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FText ItemInfoText;
};

UCLASS()
class BLADERUSH_API UInventoryItemInfoFragment : public UInventoryItemFragment
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadOnly)	
	FInventoryItemInfo InventoryItemInfo;
};
