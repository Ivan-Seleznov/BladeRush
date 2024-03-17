// Fill out your copyright notice in the Description page of Project Settings.

#include "Camera/Fragments/CameraYawLimitFragment.h"

#include "Camera/BladeRushCameraManager.h"
#include "Kismet/KismetMathLibrary.h"

void UCameraYawLimitFragment::Activate(ABladeRushCameraManager* CameraManager)
{
	Super::Activate(CameraManager);

	DefaultMaxViewYaw = CameraManager->ViewYawMax;
	DefaultMinViewYaw = CameraManager->ViewYawMin;

	CameraManager->ViewYawMax = CameraManager->GetOwningPawn()->GetActorRotation().Yaw + ViewYawLimit;
	CameraManager->ViewYawMin = CameraManager->GetOwningPawn()->GetActorRotation().Yaw - ViewYawLimit;
}

void UCameraYawLimitFragment::ResetToDefault(ABladeRushCameraManager* CameraManager)
{
	Super::ResetToDefault(CameraManager);

	CameraManager->ViewYawMax = DefaultMaxViewYaw;
	CameraManager->ViewYawMin = DefaultMinViewYaw;
}