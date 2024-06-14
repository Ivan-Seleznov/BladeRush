// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CameraMode.h"
#include "CameraMode_InterpFOV.generated.h"

/**
 * 
 */
UCLASS()
class BLADERUSH_API UCameraMode_InterpFOV : public UCameraMode
{
    GENERATED_BODY()

public:
    virtual void SetCameraManager(APlayerCameraManager* CameraManager) override;

    virtual void UpdateView(float DeltaTime, USpringArmComponent *SpringArm) override;

protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
    float TargetFOV = 100.f;

    UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
    bool bUseCustomOriginalFOV = false;
    UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(EditCondition="bUseCustomOriginalFOV"))
    float CustomOriginalFOV = 90.f;
    
    float OriginalFOV;
};
