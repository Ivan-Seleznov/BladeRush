// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "BladeRushAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class BLADERUSH_API UBladeRushAssetManager : public UAssetManager
{
	GENERATED_BODY()
public:
	virtual void StartInitialLoading() override;
};
