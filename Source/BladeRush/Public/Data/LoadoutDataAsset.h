// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Loadout/LoadoutTypes.h"
#include "LoadoutDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class BLADERUSH_API ULoadoutDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	FCharacterLoadout GetCharacterLoadout() const {return CurrentLoadout;}
	
private:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(AllowPrivateAccess))
	FCharacterLoadout CurrentLoadout;
};
