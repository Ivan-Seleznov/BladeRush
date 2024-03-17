// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/Fragments/CameraPitchLimitFragment.h"
#include "Camera/BladeRushCameraManager.h"

void UCameraPitchLimitFragment::Activate(ABladeRushCameraManager* CameraManager)
{
	Super::Activate(CameraManager);

	DefaultMaxViewPitch = CameraManager->ViewPitchMax;
	DefaultMinViewPitch = CameraManager->ViewPitchMin;
	
	CameraManager->ViewPitchMax = MaxViewPitch;
	CameraManager->ViewPitchMin = MinViewPitch;
}

void UCameraPitchLimitFragment::ResetToDefault(ABladeRushCameraManager* CameraManager)
{
	Super::ResetToDefault(CameraManager);

	CameraManager->ViewPitchMax = DefaultMaxViewPitch;
	CameraManager->ViewPitchMin = DefaultMinViewPitch;
}
