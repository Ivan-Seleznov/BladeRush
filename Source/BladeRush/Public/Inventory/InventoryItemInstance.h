// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InventoryItemInstance.generated.h"

class UInventoryItemDefinition;

/**
 * 
 */
UCLASS()
class BLADERUSH_API UInventoryItemInstance : public UObject
{
	GENERATED_BODY()
public:
	UInventoryItemInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual bool IsSupportedForNetworking() const override { return true; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, BlueprintPure=false, meta=(DeterminesOutputType=FragmentClass))
	const UInventoryItemFragment* FindFragmentByClass(TSubclassOf<UInventoryItemFragment> FragmentClass) const;
	
	void SetItemDefClass(TSubclassOf<UInventoryItemDefinition> InDef);
private:
	UPROPERTY(Replicated)
	TSubclassOf<UInventoryItemDefinition> InventoryItemDefinitionClass;
};
