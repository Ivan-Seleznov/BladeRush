// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "BladeRushCameraManager.generated.h"

/**
 * Blade rush camera manager class
 */
UCLASS()
class BLADERUSH_API ABladeRushCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	virtual void UpdateCamera(float DeltaTime) override;

protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float ViewPitch = 50.f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float ViewYaw = 50.f;

	bool ShouldSetCustomViewData() const;

	const ABladeRushCameraManager* GetDefault() const;
};
