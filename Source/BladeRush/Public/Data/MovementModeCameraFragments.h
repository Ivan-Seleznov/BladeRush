// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BladeRushGlobals.h"
#include "Engine/DataAsset.h"
#include "Camera/BladeRushCameraManager.h"
#include "MovementModeCameraFragments.generated.h"

/**
 * 
 */
UCLASS()
class BLADERUSH_API UMovementModeCameraFragments : public UDataAsset
{
	GENERATED_BODY()

public:
	const TMap<TEnumAsByte<ECustomMovementMode>,FCameraFragments>& GetActivateMovementModeFragments() const {return ActivateMovementModeFragments;}
	const TMap<TEnumAsByte<ECustomMovementMode>,FCameraFragments>& GetEndMovementModeFragments() const {return EndMovementModeFragments;}

private:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="MovementActions", meta=(AllowPrivateAccess=true))
	TMap<TEnumAsByte<ECustomMovementMode>,FCameraFragments> ActivateMovementModeFragments;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="MovementActions", meta=(AllowPrivateAccess=true))
	TMap<TEnumAsByte<ECustomMovementMode>,FCameraFragments> EndMovementModeFragments;
};
