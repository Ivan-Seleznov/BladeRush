// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Kismet/GameplayStatics.h"
#include "MapInfoDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FMapInfo
{
	GENERATED_BODY()
	
	FMapInfo()
   : FMapInfo(TEXT("SampleMap"),FText::FromString(TEXT("Sample Map")))
	{}
 
	FMapInfo(FName InMapName, FText InDisplayMapName)
	: MapName(InMapName), DisplayMapName(InDisplayMapName)
	{}
 
	FMapInfo(const FMapInfo& Other)
	: FMapInfo(Other.MapName, Other.DisplayMapName)
	{}
	
	bool operator==(const FMapInfo& Other) const
	{
		return Equals(Other);
	}
 
	bool Equals(const FMapInfo& Other) const
	{
		return MapName.IsEqual(Other.MapName);
	}
	
	UPROPERTY(EditDefaultsOnly)
	FName MapName;
	
	UPROPERTY(EditDefaultsOnly)
	FText DisplayMapName;
};

#if UE_BUILD_DEBUG
uint32 GetTypeHash(const FMapInfo& Thing);
#else 
FORCEINLINE uint32 GetTypeHash(const FMapInfo& Thing)
{
	uint32 Hash = FCrc::MemCrc32(&Thing, sizeof(FMapInfo));
	return Hash;
}
#endif

UCLASS(BlueprintType)
class BLADERUSH_API UMapInfoDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	TSoftObjectPtr<UWorld> GetMapPtrByName(FName InMapName);

	UFUNCTION(BlueprintCallable)
	TSoftObjectPtr<UWorld> GetMapPtrByDisplayName(FText InMapDisplayName);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<FText> GetAllMapDisplayNames() const;
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	TMap<FMapInfo, TSoftObjectPtr<UWorld>> Maps;
};
