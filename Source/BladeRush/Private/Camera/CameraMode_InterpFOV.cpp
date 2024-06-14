// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/CameraMode_InterpFOV.h"

#include "Characters/Player/PlayerCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

class APlayerCharacter;

void UCameraMode_InterpFOV::SetCameraManager(APlayerCameraManager* CameraManager)
{
    Super::SetCameraManager(CameraManager);
    
    
    OriginalFOV = CameraManager->GetClass()->GetDefaultObject<APlayerCameraManager>()->GetFOVAngle();
}

void UCameraMode_InterpFOV::UpdateView(float DeltaTime, USpringArmComponent* SpringArm)
{
    Super::UpdateView(DeltaTime, SpringArm);

    if (!PlayerCameraManager)
    {
        return;
    }

    const float Original = bUseCustomOriginalFOV ? CustomOriginalFOV : OriginalFOV;
    const float Target = bShouldEnter ? TargetFOV : Original;
    PlayerCameraManager->SetFOV(UKismetMathLibrary::FInterpTo(PlayerCameraManager->GetFOVAngle(),Target,DeltaTime,InterpSpeed));
}