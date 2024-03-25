// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CameraMode.h"
#include "CameraMode_InterpLocation.generated.h"


UCLASS()
class BLADERUSH_API UCameraMode_InterpLocation : public UCameraMode
{
	GENERATED_BODY()
public:
	virtual void SetCameraManager(APlayerCameraManager* CameraManager) override;

	void SetTargetTransform(const FVector& Location) {TargetLocation = Location;}

	UFUNCTION(BlueprintCallable)
	const FVector& GetTargetTransform() const { return TargetLocation;}
	UFUNCTION(BlueprintCallable)
	void SetTargetOffset(const FVector& Offset);
	
protected:
	virtual bool UpdateCameraTransition(float DeltaTime, USpringArmComponent* SpringArm) override;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FVector TargetLocation;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	bool bUseCustomOriginalLocation = false;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(EditCondition="bUseCustomOriginalTransform"))
	FVector CustomOriginalLocation;

	UPROPERTY(BlueprintReadOnly)
	FVector OriginalLocation;

	UPROPERTY(BlueprintReadOnly)
	FVector TargetOffset;
};
