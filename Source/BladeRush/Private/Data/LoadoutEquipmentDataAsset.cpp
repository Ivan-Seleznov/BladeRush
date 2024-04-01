// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/LoadoutEquipmentDataAsset.h"

const TMap<FName,TSoftClassPtr<UInventoryItemDefinition>>& ULoadoutEquipmentDataAsset::GetLoadoutEquipment() const
{
	return LoadoutEquipment;
}
