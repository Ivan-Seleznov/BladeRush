// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/BladeRushCameraManager.h"

#include "Characters/Player/PlayerCharacter.h"


void ABladeRushCameraManager::UpdateCamera(float DeltaTime)
{
	Super::UpdateCamera(DeltaTime);

	/*if (ShouldSetCustomViewData())
	{
		ViewPitchMin = -ViewPitch;
		ViewPitchMax = ViewPitch;

		ViewYawMin = -ViewYaw;
		ViewYawMax = ViewYaw;
	}
	else
	{
		const ABladeRushCameraManager* DefaultCameraManager = GetDefault();
		ViewPitchMin = DefaultCameraManager->ViewPitchMin;
		ViewPitchMax = DefaultCameraManager->ViewPitchMax;

		ViewYawMin = DefaultCameraManager->ViewYawMin;
		ViewYawMax = DefaultCameraManager->ViewYawMax;
	}*/
}

bool ABladeRushCameraManager::ShouldSetCustomViewData() const
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (!PlayerController) return false;

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerController->GetPawn());
	if (!PlayerCharacter) return false;

	return !PlayerCharacter->bUseControllerRotationYaw;
}

const ABladeRushCameraManager* ABladeRushCameraManager::GetDefault() const
{
	return GetClass()->GetDefaultObject<ABladeRushCameraManager>();
}
