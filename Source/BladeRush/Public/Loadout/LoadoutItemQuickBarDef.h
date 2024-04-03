// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LoadoutItemDefinition.h"
#include "LoadoutItemQuickBarDef.generated.h"

/**
 * Loadout Item To QuickBar Definition. Contains QuickBar slot index
 */
UCLASS()
class BLADERUSH_API ULoadoutItemQuickBarDef : public ULoadoutItemDefinition
{
	GENERATED_BODY()

public:
	int32 GetSlotIndex() const {return SlotIndex;}
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int32 SlotIndex;
};
