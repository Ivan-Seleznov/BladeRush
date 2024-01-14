// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeaponActor.generated.h"

class UWeaponItemInstance;

UCLASS()
class BLADERUSH_API ABaseWeaponActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseWeaponActor();

	FVector GetMuzzleLocation() const;
	USkeletalMeshComponent* GetWeaponMesh() const {return WeaponMesh;}
	FTransform GetHandSocketTransform() const;

	UFUNCTION(BlueprintPure)
	FTransform GetAimOffset() const;

	void OnFire(UWeaponItemInstance* WeaponInstance);
	void OnStartReloading(UWeaponItemInstance* WeaponInstance);
	void OnFinishReloading(UWeaponItemInstance* WeaponInstance);
	void OnHit(UWeaponItemInstance* WeaponInstance,TArray<FHitResult> HitResults);

	void OnEnterADS(UWeaponItemInstance* WeaponInstance);
	void OnExitADS(UWeaponItemInstance* WeaponInstance);
	
	UFUNCTION(BlueprintImplementableEvent)
	void K2_OnFire(UWeaponItemInstance* WeaponInstance);

	UFUNCTION(BlueprintImplementableEvent)
	void K2_OnEnterADS(UWeaponItemInstance* WeaponInstance);
	UFUNCTION(BlueprintImplementableEvent)
	void K2_OnExitADS(UWeaponItemInstance* WeaponInstance);

protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	USceneComponent* AimOffsetSceneComponent;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UAnimMontage* CharacterFireMontage;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UAnimMontage* WeaponFireMontage;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<UCameraShakeBase> FireCameraShake;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<UCameraShakeBase> ADSFireCameraShake;
private:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(AllowPrivateAccess))
	FName MuzzleSocketName = FName("muzzle_socket");

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(AllowPrivateAccess))
	FName LeftHandIKSocketName = FName("LHIK");
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(AllowPrivateAccess))
	FName RightHandSocketName = FName("hand_r");
	
	void PlayCameraShake(TSubclassOf<UCameraShakeBase> CameraShakeClass);
	void PlayWeaponAnimMontage(UAnimMontage* Montage) const;
	void PlayCharacterAnimMontage(UAnimMontage* Montage) const;
};
