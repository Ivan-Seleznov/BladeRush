// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CameraMode.h"
#include "CameraMode_InterpTransform.generated.h"


UCLASS()
class BLADERUSH_API UCameraMode_InterpTransform : public UCameraMode
{
	GENERATED_BODY()
public:
	virtual void SetCameraManager(APlayerCameraManager* CameraManager) override;

	void SetTargetTransform(const FTransform& Transform) {TargetTransform = Transform;}

	UFUNCTION(BlueprintCallable)
	const FTransform& GetTargetTransform() const { return TargetTransform;}
	UFUNCTION(BlueprintCallable)
	void SetTargetTransformOffset(const FTransform& Offset);
protected:
	virtual bool UpdateCameraTransition(float DeltaTime, USpringArmComponent* SpringArm) override;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FTransform TargetTransform;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	bool bUseCustomOriginalTransform = false;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(EditCondition="bUseCustomOriginalTransform"))
	FTransform CustomOriginalTransform;

	UPROPERTY(BlueprintReadOnly)
	FTransform OriginalTransform;

	UPROPERTY(BlueprintReadOnly)
	FTransform TargetOffset;
};
