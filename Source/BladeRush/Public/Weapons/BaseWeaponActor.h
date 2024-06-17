// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageCauserInterface.h"
#include "GameFramework/Actor.h"
#include "Inventory/InventoryItemInstance.h"
#include "BaseWeaponActor.generated.h"

class UHitMarkerWidgetBase;
class UWeaponItemInstance;
class UDecalDataAsset;
class UNiagaraSystem;
class UCameraMode_InterpLocation;

UCLASS()
class BLADERUSH_API ABaseWeaponActor : public AActor, public IDamageCauserInterface
{
	GENERATED_BODY()
	
public:	
	ABaseWeaponActor();

	virtual const FSlateBrush& GetCauserIcon() const override { return WeaponIcon; };

	FVector GetMuzzleLocation() const;
	FRotator GetMuzzleRotation() const;

	USkeletalMeshComponent* GetWeaponMesh() const {return WeaponMesh;}
	FTransform GetHandSocketTransform() const;

	UFUNCTION(BlueprintPure)
	FTransform GetAimOffset() const;
	
	void OnStartReloading(UWeaponItemInstance* WeaponInstance);
	void OnFinishReloading(UWeaponItemInstance* WeaponInstance);
	
	void OnClientHit(UWeaponItemInstance* WeaponInstance,TArray<FHitResult> HitResults);
	void OnServerHit(UWeaponItemInstance* WeaponInstance,const TArray<FHitResult>& HitResults);
	
	void OnEnterADS(UWeaponItemInstance* WeaponInstance);
	void OnExitADS(UWeaponItemInstance* WeaponInstance);
	
	UFUNCTION(BlueprintImplementableEvent)
	void K2_ClientHit(UWeaponItemInstance* WeaponInstance);

	UFUNCTION(BlueprintImplementableEvent)
	void K2_HandleHit(FHitResult Hit);
	
	UFUNCTION(BlueprintImplementableEvent)
	void K2_OnEnterADS(UWeaponItemInstance* WeaponInstance);
	UFUNCTION(BlueprintImplementableEvent)
	void K2_OnExitADS(UWeaponItemInstance* WeaponInstance);

	TSubclassOf<UCameraMode_InterpLocation> GetCameraModeClass() const {return CameraModeClass;}
	
	USceneComponent* GetScopeAttachPoint() const {return ScopeAttachPoint;}
protected:
	virtual void Destroyed() override;
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintPure)
	ACharacter* GetCharacterOwner() const;

	template<typename T>
	T* GetCharacterOwner() const;
	
	UFUNCTION(NetMulticast,Unreliable)
	void OnHit_Multicast(UWeaponItemInstance* WeaponInstance,const TArray<FHitResult>& HitResults);

	void HandleHits(UWeaponItemInstance* WeaponInstance,const TArray<FHitResult>& HitResults);

	void HandleHit(UWeaponItemInstance* WeaponInstance, const FHitResult& Hit);
	void SpawnDecal(const FHitResult& Hit);
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	USceneComponent* AimOffsetSceneComponent;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	USceneComponent* ScopeAttachPoint;
	
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

	UPROPERTY(EditDefaultsOnly, Category = "Decals")
	UDecalDataAsset* DecalData;
	UPROPERTY(EditDefaultsOnly, Category = "Decals")
	FVector DecalSize = {10.f,10.f,10.f};
	UPROPERTY(EditDefaultsOnly, Category = "Decals")
	float DecalLifeTime = 4.f;

	UPROPERTY(EditDefaultsOnly, Category = "Particles")
	UParticleSystem* HitParticles;
	UPROPERTY(EditDefaultsOnly, Category = "Particles")
	UParticleSystem* MuzzleFlashParticles;
	UPROPERTY(EditDefaultsOnly, Category = "Particles")
	FTransform MuzzleFlashOffset = FTransform::Identity;

	UPROPERTY(EditDefaultsOnly, Category = "Particles")
	UNiagaraSystem* TracerParticles;
	UPROPERTY(EditDefaultsOnly, Category = "Particles")
	FTransform TracerParticlesOffset = FTransform::Identity;
	UPROPERTY(EditDefaultsOnly, Category = "Particles")
	FName TracerParticlesEndPointParam = "BeamEnd";
	
	UPROPERTY(EditDefaultsOnly, Category = "Sounds", meta=(AllowPrivateAccess=true))
	USoundBase* ShootSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds", meta=(AllowPrivateAccess=true))
	USoundBase* HitSurfaceSound;

	UPROPERTY(EditDefaultsOnly, Category="HitMarker")
	TSubclassOf<UHitMarkerWidgetBase> HitMarkerWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category="HitMarker")
	USoundBase* HitMarkerSound;
	UPROPERTY(EditDefaultsOnly, Category="HitMarker")
	float HitMarkerSoundVolumeMultiplier = 1.3f;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Camera")	
	TSubclassOf<UCameraMode_InterpLocation> CameraModeClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI")
	FSlateBrush WeaponIcon;
	
private:
	void PlayCameraShake(TSubclassOf<UCameraShakeBase> CameraShakeClass);
	void PlayWeaponAnimMontage(UAnimMontage* Montage) const;
	void PlayCharacterAnimMontage(UAnimMontage* Montage) const;
	void AddHitMarker();
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(AllowPrivateAccess))
	FName MuzzleSocketName = FName("muzzle_socket");

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(AllowPrivateAccess))
	FName LeftHandIKSocketName = FName("LHIK");
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(AllowPrivateAccess))
	FName RightHandSocketName = FName("hand_r");
	
	UPROPERTY(EditDefaultsOnly, Category = "Debug", meta=(AllowPrivateAccess=true))
	bool bDrawDebugHits = true;

	UPROPERTY()
	UHitMarkerWidgetBase* HitMarkerWidget;
};

template <typename T>
T* ABaseWeaponActor::GetCharacterOwner() const
{
	return Cast<T>(GetOwner());
}
