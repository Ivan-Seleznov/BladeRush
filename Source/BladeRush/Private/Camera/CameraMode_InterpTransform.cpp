// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/CameraMode_InterpTransform.h"

#include "Characters/Player/PlayerCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"


void UCameraMode_InterpTransform::SetCameraManager(APlayerCameraManager* CameraManager)
{
	Super::SetCameraManager(CameraManager);

	APlayerController* PlayerController = CameraManager->GetOwningPlayerController();
	if (!PlayerController) return;

	if (!PlayerController->GetCharacter()) return;

	const APlayerCharacter* DefaultCharacter = PlayerController->GetCharacter()->GetClass()->GetDefaultObject<APlayerCharacter>();
	OriginalTransform = DefaultCharacter->GetSpringArm()->GetRelativeTransform();
}

void UCameraMode_InterpTransform::SetTargetTransformOffset(const FTransform& Offset)
{
	TargetOffset = Offset;
	TargetTransform += Offset;
}

bool UCameraMode_InterpTransform::UpdateCameraTransition(float DeltaTime, USpringArmComponent* SpringArm)
{
	Super::UpdateCameraTransition(DeltaTime, SpringArm);

	const FTransform CurrentTransform = SpringArm->GetRelativeTransform();

	const FTransform Original = bUseCustomOriginalTransform ? CustomOriginalTransform : OriginalTransform;
	const FTransform Target = bShouldEnter ? TargetTransform : Original;
	
	SpringArm->SetRelativeTransform(UKismetMathLibrary::TInterpTo(CurrentTransform,Target,DeltaTime,InterpSpeed));

	return !SpringArm->GetRelativeTransform().Equals(Target);
}
