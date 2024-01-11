// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/BladeRushAnimInstance.h"

#include "Kismet/KismetMathLibrary.h"
#include "Weapons/WeaponItemInstance.h"

void UBladeRushAnimInstance::AddRecoil(const FWeaponRecoilData& WeaponRecoilData)
{
	FRotator RecoilRotation;
	RecoilRotation.Roll = FMath::RandRange(-1.f,-2.5f) * WeaponRecoilData.RecoilMultiplier;
	RecoilRotation.Pitch = FMath::RandRange(-0.4f,0.4f) * WeaponRecoilData.SideRecoilMultiplier;
	RecoilRotation.Yaw = FMath::RandRange(-0.8f,0.8f) * WeaponRecoilData.SideRecoilMultiplier;

	FVector RecoilLocation;
	RecoilLocation.X = FMath::RandRange(-0.1f,-0.1f) * WeaponRecoilData.MoveXMultiplier;
	RecoilLocation.Y = FMath::RandRange(-1.f,-2.f) * WeaponRecoilData.MoveYMultiplier;
	RecoilLocation.Z = FMath::RandRange(-0.5f,1.f) * WeaponRecoilData.MoveZMultiplier;

	RecoilTransform.SetLocation(RecoilLocation);
	RecoilTransform.SetRotation(RecoilRotation.Quaternion());
}

void UBladeRushAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	Recoil = UKismetMathLibrary::TInterpTo(Recoil,RecoilTransform,DeltaSeconds,25.f);
	RecoilTransform = UKismetMathLibrary::TInterpTo(RecoilTransform,FTransform::Identity,DeltaSeconds,15.f);
}
