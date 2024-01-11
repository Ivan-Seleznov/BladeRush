// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BladeRushAnimInstance.generated.h"

struct FWeaponRecoilData;
/**
 * 
 */
UCLASS()
class BLADERUSH_API UBladeRushAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	void AddRecoil(const FWeaponRecoilData& WeaponRecoilData);

	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(BlueprintReadOnly)
	FTransform RecoilTransform;

	UPROPERTY(BlueprintReadOnly)
	FTransform Recoil;
};
