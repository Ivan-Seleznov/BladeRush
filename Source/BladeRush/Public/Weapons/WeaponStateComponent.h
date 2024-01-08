// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ControllerComponent.h"
#include "WeaponStateComponent.generated.h"

/**
 * Weapon state component
 */
UCLASS(Blueprintable,BlueprintType,meta=(BlueprintSpawnableComponent))
class BLADERUSH_API UWeaponStateComponent : public UControllerComponent
{
	GENERATED_BODY()

public:
	UWeaponStateComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//virtual void PlayerTick(float DeltaTime) override;
};
