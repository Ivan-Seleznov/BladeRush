// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseCameraFragment.generated.h"

class ABladeRushCameraManager;

/**
 * 
 */
UCLASS(DefaultToInstanced, EditInlineNew, Abstract, BlueprintType,Blueprintable)
class BLADERUSH_API UBaseCameraFragment : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void Activate(ABladeRushCameraManager* CameraManager);
	UFUNCTION()
	virtual void ResetToDefault(ABladeRushCameraManager* CameraManager);
	
	UFUNCTION(BlueprintImplementableEvent)
	void K2_Activate(ABladeRushCameraManager* CameraManager);

	UFUNCTION(BlueprintPure)
	bool ShouldResetToDefault() const {return bShouldResetToDefault;}
private:
	UPROPERTY(EditDefaultsOnly,meta=(AllowPrivateAccess=true))
	bool bShouldResetToDefault = false;
};
