// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equipment/EquipmentInstance.h"
#include "WeaponItemInstance.generated.h"

/**
 * 
 */
UCLASS()
class BLADERUSH_API UWeaponItemInstance : public UEquipmentInstance
{
	GENERATED_BODY()
public:
	UWeaponItemInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void OnEquipped() override;
    virtual void OnUnequipped() override;

	AActor* GetSpawnedWeaponActor() const;
};
