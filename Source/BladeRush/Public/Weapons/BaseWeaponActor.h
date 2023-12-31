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

	void OnFire(UWeaponItemInstance* WeaponInstance);
	void OnStartReloading(UWeaponItemInstance* WeaponInstance);
	void OnFinishReloading(UWeaponItemInstance* WeaponInstance);
	void OnHit(UWeaponItemInstance* WeaponInstance,TArray<FHitResult> HitResults);

	UFUNCTION(BlueprintImplementableEvent)
	void K2_OnFire(UWeaponItemInstance* WeaponInstance);
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	UAnimMontage* CharacterFireMontage;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	UAnimMontage* WeaponFireMontage;
private:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(AllowPrivateAccess))
	FName MuzzleSocketName = FName("muzzle_socket");

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(AllowPrivateAccess))
	FName LeftHandIKSocketName = FName("LHIK");
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(AllowPrivateAccess))
	FName RightHandSocketName = FName("hand_r");

	
};
