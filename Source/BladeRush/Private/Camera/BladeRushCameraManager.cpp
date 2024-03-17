// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/BladeRushCameraManager.h"
#include "BladeRushLogs.h"
#include "Camera/CameraMode.h"
#include "Camera/Fragments/BaseCameraFragment.h"
#include "Characters/Components/ShooterMovementComponent.h"
#include "Characters/Player/PlayerCharacter.h"
#include "Data/MovementActionCameraFragments.h"
#include "Data/MovementModeCameraFragments.h"


void ABladeRushCameraManager::InitCameraManagerPawn(APawn* Pawn)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Pawn);
	if (!PlayerCharacter)
	{
		return;
	}

	UShooterMovementComponent* ShooterMovementComponent = PlayerCharacter->GetShooterMovementComponent();
	if (!ShooterMovementComponent)
	{
		return;
	}
	
	ShooterMovementComponent->MovementActionStartedDelegate.RemoveAll(this);
	ShooterMovementComponent->MovementActionEndedDelegate.RemoveAll(this);
	PlayerCharacter->MovementModeChangedDelegate.RemoveAll(this);
	
	ShooterMovementComponent->MovementActionStartedDelegate.AddDynamic(this,&ThisClass::OnMovementActionStarted);
	ShooterMovementComponent->MovementActionEndedDelegate.AddDynamic(this,&ThisClass::OnMovementActionEnded);

	PlayerCharacter->MovementModeChangedDelegate.AddDynamic(this,&ThisClass::OnMovementModeChanged);
}

void ABladeRushCameraManager::UpdateCamera(float DeltaTime)
{
	Super::UpdateCamera(DeltaTime);


	const APlayerController* PlayerController = GetOwningPlayerController();
	if (!PlayerController)
	{
		return;
	}

	const APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerController->GetPawn());
	if (!PlayerCharacter)
	{
		return;
	}

	if (CurrentCameraMode)
	{
		CurrentCameraMode->UpdateView(DeltaTime,PlayerCharacter->GetSpringArm());
	}
}

void ABladeRushCameraManager::SetCameraMode(TSubclassOf<UCameraMode> CameraModeClass, bool bShouldEnterCameraMode)
{
	if (!CameraModeClass)
	{
		return;
	}

	APlayerController* PlayerController = GetOwningPlayerController();
	if (!PlayerController)
	{
		return;
	}

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerController->GetPawn());
	if (!PlayerCharacter)
	{
		return;
	}
	
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
	if (!CameraMode)
	{
		return;
	}

	APlayerController* PlayerController = GetOwningPlayerController();
	if (!PlayerController)
	{
		return;
	}

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerController->GetPawn());
	if (!PlayerCharacter)
	{
		return;
	}
	
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

APawn* ABladeRushCameraManager::GetOwningPawn() const
{
	if (const APlayerController* PlayerController = GetOwningPlayerController())
	{
		return PlayerController->GetPawn();
	}

	return nullptr;
}

void ABladeRushCameraManager::ClearCameraMode()
{
	CurrentCameraMode = nullptr;
}

bool ABladeRushCameraManager::ShouldSetCustomViewData() const
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (!PlayerController)
	{
		return false;
	}

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerController->GetPawn());
	if (!PlayerCharacter)
	{
		return false;
	}

	return !PlayerCharacter->bUseControllerRotationYaw;
}

const ABladeRushCameraManager* ABladeRushCameraManager::GetDefault() const
{
	return GetClass()->GetDefaultObject<ABladeRushCameraManager>();
}

void ABladeRushCameraManager::OnMovementActionStarted(TEnumAsByte<EMovementAction> NewMovementAction,
	TEnumAsByte<EMovementAction> PrevMovementAction)
{
	if (NewMovementAction == EMovementAction::None || !MovementActionCameraFragments)
	{
		return;
	}

	const TMap<TEnumAsByte<EMovementAction>,FCameraFragments>& ActivateMovementActionFragments = MovementActionCameraFragments->GetActivateMovementActionFragments();
	if (ActivateMovementActionFragments.Contains(NewMovementAction))
	{
		ActivateCameraFragments(ActivateMovementActionFragments[NewMovementAction]);
	}
}

void ABladeRushCameraManager::OnMovementActionEnded(TEnumAsByte<EMovementAction> EndedMovementAction)
{
	if (EndedMovementAction == EMovementAction::None || !MovementActionCameraFragments)
	{
		return;
	}

	const TMap<TEnumAsByte<EMovementAction>,FCameraFragments>& ActivateMovementActionFragments = MovementActionCameraFragments->GetActivateMovementActionFragments();
	if (ActivateMovementActionFragments.Contains(EndedMovementAction))
	{
		ResetCameraFragmentsToDefault(ActivateMovementActionFragments[EndedMovementAction]);
	}

	const TMap<TEnumAsByte<EMovementAction>,FCameraFragments>& EndMovementActionFragments = MovementActionCameraFragments->GetEndMovementActionFragments();
	if (EndMovementActionFragments.Contains(EndedMovementAction))
	{
		ActivateCameraFragments(EndMovementActionFragments[EndedMovementAction]);
	}
}

void ABladeRushCameraManager::OnMovementModeChanged(ACharacter* Character, EMovementMode PrevMovementMode,
	uint8 PreviousCustomMode)
{
	if (!MovementModeCameraFragments)
	{
		return;
	}

	const UShooterMovementComponent* MovementComponent = Cast<UShooterMovementComponent>(Character->GetMovementComponent());
	if (!MovementComponent)
	{
		return;
	}

	
	const TEnumAsByte<ECustomMovementMode> PrevCustomMode = static_cast<TEnumAsByte<ECustomMovementMode>>(PreviousCustomMode);

	const TMap<TEnumAsByte<ECustomMovementMode>,FCameraFragments>& EndMovementModeCameraFragments = MovementModeCameraFragments->GetEndMovementModeFragments();
	const TMap<TEnumAsByte<ECustomMovementMode>,FCameraFragments>& ActivateMovementModeCameraFragments = MovementModeCameraFragments->GetActivateMovementModeFragments();

	if (PrevMovementMode == MOVE_Custom)
	{
		if (ActivateMovementModeCameraFragments.Contains(PrevCustomMode))
		{
			ResetCameraFragmentsToDefault(ActivateMovementModeCameraFragments[PrevCustomMode]);
		}
		if (EndMovementModeCameraFragments.Contains(PrevCustomMode))
		{
			ActivateCameraFragments(EndMovementModeCameraFragments[PrevCustomMode]);
		}
	}

	if (MovementComponent->MovementMode != MOVE_Custom)
	{
		return;
	}
	
	const TEnumAsByte<ECustomMovementMode> CurrentCustomMode = static_cast<TEnumAsByte<ECustomMovementMode>>(MovementComponent->CustomMovementMode);

	if (ActivateMovementModeCameraFragments.Contains(CurrentCustomMode))
	{
		ActivateCameraFragments(ActivateMovementModeCameraFragments[CurrentCustomMode]);
	}
}

void ABladeRushCameraManager::ActivateCameraFragments(const FCameraFragments& CameraFragments)
{
	for (UBaseCameraFragment* CameraFragment : CameraFragments.CameraFragments)
	{
		if (CameraFragment)
		{
			CameraFragment->Activate(this);
		}
	}
}

void ABladeRushCameraManager::ResetCameraFragmentsToDefault(const FCameraFragments& CameraFragments)
{
	for (UBaseCameraFragment* CameraFragment : CameraFragments.CameraFragments)
	{
		if (CameraFragment && CameraFragment->ShouldResetToDefault())
		{
			CameraFragment->ResetToDefault(this);
		}
	}
}
