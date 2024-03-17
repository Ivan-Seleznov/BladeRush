// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "Characters/Components/ShooterMovementComponent.h"
#include "BladeRushCameraManager.generated.h"

class UMovementActionCameraFragments;
class UBaseCameraFragment;
class UCameraMode;
class UMovementModeCameraFragments;

/**
 * Blade rush camera manager class
 */
USTRUCT(BlueprintType)
struct FCameraFragments
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Fragments",meta=(AllowPrivateAccess=true))
	TArray<TObjectPtr<UBaseCameraFragment>> CameraFragments;
};
UCLASS()
class BLADERUSH_API ABladeRushCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	virtual void InitCameraManagerPawn(APawn* Pawn);
	
	virtual void UpdateCamera(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetCameraMode(TSubclassOf<UCameraMode> CameraModeClass,bool bShouldEnterCameraMode = true);
	UFUNCTION(BlueprintCallable)
	void SetCameraModeObject(UCameraMode* CameraMode, bool bShouldEnterCameraMode = true);
	
	UFUNCTION(BlueprintCallable)
	void EnterCurrentCameraMode();

	UFUNCTION(BlueprintCallable)
	void ExitCurrentCameraMode();

	UFUNCTION(BlueprintPure)
	UCameraMode* GetCurrentCameraMode() const {return CurrentCameraMode;}

	UFUNCTION(BlueprintPure)
	APawn* GetOwningPawn() const;
	
	UFUNCTION(BlueprintCallable)
	void ClearCameraMode();
	
protected:
	bool ShouldSetCustomViewData() const;

	const ABladeRushCameraManager* GetDefault() const;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "CameraMode")
	TObjectPtr<UCameraMode> CurrentCameraMode;

private:
	UFUNCTION()
	void OnMovementActionStarted(TEnumAsByte<EMovementAction> NewMovementAction, TEnumAsByte<EMovementAction> PrevMovementAction);
	UFUNCTION()
	void OnMovementActionEnded(TEnumAsByte<EMovementAction> EndedMovementAction);

	UFUNCTION()
	void OnMovementModeChanged(ACharacter* Character,EMovementMode PrevMovementMode, uint8 PreviousCustomMode);
	
	void ActivateCameraFragments(const FCameraFragments& CameraFragments);
	void ResetCameraFragmentsToDefault(const FCameraFragments& CameraFragments);

	UPROPERTY(EditDefaultsOnly,meta=(AllowPrivateAccess=true))
	TObjectPtr<UMovementActionCameraFragments> MovementActionCameraFragments;

	UPROPERTY(EditDefaultsOnly,meta=(AllowPrivateAccess=true))
	TObjectPtr<UMovementModeCameraFragments> MovementModeCameraFragments;
};
