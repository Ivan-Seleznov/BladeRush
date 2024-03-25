// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/CameraMode_InterpLocation.h"

#include "Characters/Player/PlayerCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"


void UCameraMode_InterpLocation::SetCameraManager(APlayerCameraManager* CameraManager)
{
	Super::SetCameraManager(CameraManager);

	APlayerController* PlayerController = CameraManager->GetOwningPlayerController();
	if (!PlayerController) return;

	if (!PlayerController->GetCharacter()) return;

	const APlayerCharacter* DefaultCharacter = PlayerController->GetCharacter()->GetClass()->GetDefaultObject<APlayerCharacter>();
	OriginalLocation = DefaultCharacter->GetSpringArm()->GetRelativeLocation();
}

void UCameraMode_InterpLocation::SetTargetOffset(const FVector& Offset)
{
	TargetOffset = Offset;
	TargetLocation += Offset;
}

bool UCameraMode_InterpLocation::UpdateCameraTransition(float DeltaTime, USpringArmComponent* SpringArm)
{
	Super::UpdateCameraTransition(DeltaTime, SpringArm);

	const FVector CurrentLocation = SpringArm->GetRelativeLocation();

	const FVector Original = bUseCustomOriginalLocation ? CustomOriginalLocation : OriginalLocation;
	const FVector Target = bShouldEnter ? TargetLocation : Original;
	
	SpringArm->SetRelativeLocation(UKismetMathLibrary::VInterpTo(CurrentLocation,Target,DeltaTime,InterpSpeed));

	return !SpringArm->GetRelativeLocation().Equals(Target);
}
