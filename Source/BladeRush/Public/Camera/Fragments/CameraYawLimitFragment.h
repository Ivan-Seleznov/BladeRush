// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCameraFragment.h"
#include "CameraYawLimitFragment.generated.h"

/**
 * 
 */
UCLASS()
class BLADERUSH_API UCameraYawLimitFragment : public UBaseCameraFragment
{
	GENERATED_BODY()

public:
	virtual void Activate(ABladeRushCameraManager* CameraManager) override;
	virtual void ResetToDefault(ABladeRushCameraManager* CameraManager) override;
	
private:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(AllowPrivateAccess=true))
	float ViewYawMaxLimit = 0.f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(AllowPrivateAccess=true))
	float ViewYawMinLimit = 0.f;
	
	UPROPERTY(BlueprintReadOnly,meta=(AllowPrivateAccess=true))
	float DefaultMaxViewYaw;
	UPROPERTY(BlueprintReadOnly,meta=(AllowPrivateAccess=true))
	float DefaultMinViewYaw;
};
