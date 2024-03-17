// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/Fragments/BaseCameraFragment.h"

void UBaseCameraFragment::Activate(ABladeRushCameraManager* CameraManager)
{
	if (!CameraManager)
	{
		return;
	}
	
	K2_Activate(CameraManager);
}

void UBaseCameraFragment::ResetToDefault(ABladeRushCameraManager* CameraManager)
{
}

