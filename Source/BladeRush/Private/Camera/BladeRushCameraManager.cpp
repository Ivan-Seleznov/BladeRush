// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/BladeRushCameraManager.h"
#include "BladeRushLogs.h"
#include "Camera/CameraMode.h"
#include "Characters/Player/PlayerCharacter.h"


void ABladeRushCameraManager::UpdateCamera(float DeltaTime)
{
	Super::UpdateCamera(DeltaTime);


	APlayerController* PlayerController = GetOwningPlayerController();
	if (!PlayerController) return;

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerController->GetPawn());
	if (!PlayerCharacter) return;

	if (CurrentCameraMode)
	{
		CurrentCameraMode->UpdateView(DeltaTime,PlayerCharacter->GetSpringArm());
	}
}

void ABladeRushCameraManager::SetCameraMode(TSubclassOf<UCameraMode> CameraModeClass, bool bShouldEnterCameraMode)
{
	if (!CameraModeClass) return;

	APlayerController* PlayerController = GetOwningPlayerController();
	if (!PlayerController) return;

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerController->GetPawn());
	if (!PlayerCharacter) return;
	
	UCameraMode* CameraMode = CameraModeClass.GetDefaultObject();
	CameraMode->SetCameraManager(this);
	
	CurrentCameraMode = CameraModeClass.GetDefaultObject();

	if (bShouldEnterCameraMode)
	{
		EnterCurrentCameraMode();
	}
}

void ABladeRushCameraManager::SetCameraModeObject(UCameraMode* CameraMode, bool bShouldEnterCameraMode)
{
	if (!CameraMode) return;

	APlayerController* PlayerController = GetOwningPlayerController();
	if (!PlayerController) return;

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerController->GetPawn());
	if (!PlayerCharacter) return;
	
	CameraMode->SetCameraManager(this);
	
	CurrentCameraMode = CameraMode;
	if (bShouldEnterCameraMode)
	{
		EnterCurrentCameraMode();
	}
}

void ABladeRushCameraManager::EnterCurrentCameraMode()
{
	if (CurrentCameraMode)
	{
		CurrentCameraMode->EnterCameraMode();
	}
	else
	{
		DEBUG_LOG("Try to enter camera mode which doesn't exist");
	}
}

void ABladeRushCameraManager::ExitCurrentCameraMode()
{
	if (CurrentCameraMode)
	{
		CurrentCameraMode->ExitCameraMode();
	}
	else
	{
		DEBUG_LOG("Try to exit camera mode which doesn't exist");
	}
}

void ABladeRushCameraManager::ClearCameraMode()
{
	CurrentCameraMode = nullptr;
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
