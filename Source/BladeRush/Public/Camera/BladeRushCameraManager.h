// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "BladeRushCameraManager.generated.h"

class UCameraMode;
/**
 * Blade rush camera manager class
 */
UCLASS()
class BLADERUSH_API ABladeRushCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	virtual void UpdateCamera(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetCameraMode(TSubclassOf<UCameraMode> CameraModeClass,bool bShouldEnterCameraMode = true);
	UFUNCTION(BlueprintCallable)
	void SetCameraModeObject(UCameraMode* CameraMode, bool bShouldEnterCameraMode = true);
	
	UFUNCTION(BlueprintCallable)
	void EnterCurrentCameraMode();

	UFUNCTION(BlueprintCallable)
	void ExitCurrentCameraMode();

	UFUNCTION(BlueprintCallable)
	UCameraMode* GetCurrentCameraMode() const {return CurrentCameraMode;}

	UFUNCTION(BlueprintCallable)
	void ClearCameraMode();
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float ViewPitch = 50.f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float ViewYaw = 50.f;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "CameraMode")
	UCameraMode* CurrentCameraMode;
	
	bool ShouldSetCustomViewData() const;

	const ABladeRushCameraManager* GetDefault() const;
};
