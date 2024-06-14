// Fill out your copyright notice in the Description page of Project Settings.


#include "MapInfoDataAsset.h"

TSoftObjectPtr<UWorld> UMapInfoDataAsset::GetMapPtrByName(FName InMapName)
{
	checkf(InMapName.IsNone(), TEXT("Map name is empty"));

	for (auto Map : Maps)
	{
		if (Map.Key.MapName == InMapName)
		{
			return Map.Value;
		}
	}
	return TSoftObjectPtr<UWorld>();
}
// TODO: OVERRIDE INDEX OPERATOR, DELETE METHODS BELOW 
TSoftObjectPtr<UWorld> UMapInfoDataAsset::GetMapPtrByDisplayName(FText InMapDisplayName)
{
	//checkf(InMapDisplayName.IsEmpty(), TEXT("Map name is empty"));

	for (auto Map : Maps)
	{
		if (Map.Key.DisplayMapName.EqualTo(InMapDisplayName))
		{
			return Map.Value;
		}
	}
	return TSoftObjectPtr<UWorld>();
}

TArray<FText> UMapInfoDataAsset::GetAllMapDisplayNames() const
{
	TArray<FText> OutMapNames;
	OutMapNames.Reserve(Maps.Num());

	for (auto Map : Maps)
	{
		OutMapNames.Add(Map.Key.DisplayMapName);
	}
	return OutMapNames;
}
