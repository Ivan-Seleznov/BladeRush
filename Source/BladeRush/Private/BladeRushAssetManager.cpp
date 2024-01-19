// Fill out your copyright notice in the Description page of Project Settings.


#include "BladeRushAssetManager.h"

#include "AbilitySystemGlobals.h"
void UBladeRushAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	UAbilitySystemGlobals::Get().InitGlobalData();
}
