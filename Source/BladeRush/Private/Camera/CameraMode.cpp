

#include "Camera/CameraMode.h"
#include "GameFramework/SpringArmComponent.h"

void UCameraMode::SetCameraManager(APlayerCameraManager* CameraManager)
{
	PlayerCameraManager = CameraManager;
}

void UCameraMode::UpdateView(float DeltaTime,USpringArmComponent* SpringArm)
{
	if (!bTransitionCamera) return;
	
	if (!UpdateCameraTransition(DeltaTime,SpringArm))
	{
		bTransitionCamera = false;
		OnCameraTransitionFinished.ExecuteIfBound(bShouldEnter);
	}
}

void UCameraMode::EnterCameraMode()
{
	bShouldEnter = true;
	bTransitionCamera = true;
}

void UCameraMode::ExitCameraMode()
{
	bShouldEnter = false;
	bTransitionCamera = true;
}

bool UCameraMode::UpdateCameraTransition(float DeltaTime, USpringArmComponent* SpringArm)
{
	return true;
}
