// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCameraFragment.h"
#include "CameraPitchLimitFragment.generated.h"

/**
 * 
 */
UCLASS()
class BLADERUSH_API UCameraPitchLimitFragment : public UBaseCameraFragment
{
	GENERATED_BODY()

public:
	virtual void Activate(ABladeRushCameraManager* CameraManager) override;
	virtual void ResetToDefault(ABladeRushCameraManager* CameraManager) override;
	
private:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(AllowPrivateAccess=true))
	float MaxViewPitch = 0.f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(AllowPrivateAccess=true))
	float MinViewPitch = 0.f;

	
	UPROPERTY(BlueprintReadOnly,meta=(AllowPrivateAccess=true))
	float DefaultMaxViewPitch;
	UPROPERTY(BlueprintReadOnly,meta=(AllowPrivateAccess=true))
	float DefaultMinViewPitch;
};
