// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BladeRushGlobals.h"
#include "Camera/BladeRushCameraManager.h"
#include "Engine/DataAsset.h"
#include "MovementActionCameraFragments.generated.h"

/**
 * 
 */
UCLASS()
class BLADERUSH_API UMovementActionCameraFragments : public UDataAsset
{
	GENERATED_BODY()
public:
	const TMap<TEnumAsByte<EMovementAction>,FCameraFragments>& GetActivateMovementActionFragments() const {return ActivateMovementActionFragments;}
	const TMap<TEnumAsByte<EMovementAction>,FCameraFragments>& GetEndMovementActionFragments() const {return EndMovementActionFragments;}

private:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="MovementActions", meta=(AllowPrivateAccess=true))
	TMap<TEnumAsByte<EMovementAction>,FCameraFragments> ActivateMovementActionFragments;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="MovementActions", meta=(AllowPrivateAccess=true))
	TMap<TEnumAsByte<EMovementAction>,FCameraFragments> EndMovementActionFragments;
};
