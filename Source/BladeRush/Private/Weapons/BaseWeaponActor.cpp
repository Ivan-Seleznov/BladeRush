// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/BaseWeaponActor.h"

#include "Characters/BaseCharacter.h"
#include "GameFramework/Character.h"

ABaseWeaponActor::ABaseWeaponActor()
{
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon Mesh");
	WeaponMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	
	RootComponent = WeaponMesh;
}

FVector ABaseWeaponActor::GetMuzzleLocation() const
{
	check(WeaponMesh);

	return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

FTransform ABaseWeaponActor::GetHandSocketTransform() const
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || !Character->GetMesh()) return FTransform::Identity;
	
	const FTransform LeftIKTransform = WeaponMesh->GetSocketTransform(LeftHandIKSocketName);

	FVector OutLoc;
	FRotator OutRot;
	Character->GetMesh()->TransformToBoneSpace(RightHandSocketName,LeftIKTransform.GetLocation(),LeftIKTransform.GetRotation().Rotator(),OutLoc,OutRot);

	FTransform Result;
	Result.SetLocation(OutLoc);
	Result.SetRotation(OutRot.Quaternion());
	Result.SetScale3D(FVector(1,1,1));
	
	GEngine->AddOnScreenDebugMessage(-1,0,FColor::Green,FString::Printf(TEXT("%s"),*Result.ToString()));
	UE_LOG(LogTemp,Display,TEXT("Transform: %s"),*Result.ToString());
	return Result;
}

void ABaseWeaponActor::OnFire(UWeaponItemInstance* WeaponInstance)
{
	ABaseCharacter* Character = Cast<ABaseCharacter>(GetOwner());
	if (!Character || !Character->GetMesh()) return;

	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	if (!AnimInstance) return;

	AnimInstance->Montage_Play(CharacterFireMontage);
	K2_OnFire(WeaponInstance);
}

void ABaseWeaponActor::OnStartReloading(UWeaponItemInstance* WeaponInstance)
{
}

void ABaseWeaponActor::OnFinishReloading(UWeaponItemInstance* WeaponInstance)
{
}

void ABaseWeaponActor::OnHit(UWeaponItemInstance* WeaponInstance, TArray<FHitResult> HitResults)
{
}
