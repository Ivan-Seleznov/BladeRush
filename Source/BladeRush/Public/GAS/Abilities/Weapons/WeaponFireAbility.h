// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWeaponAbility.h"
#include "WeaponFireAbility.generated.h"

class UWeaponItemInstance;

USTRUCT()
struct FWeaponTraceData
{
	GENERATED_BODY()

	FVector CameraLocation;
	FVector CameraDirection;
	
	FVector WeaponLocation;
};

/**
 * Base weapon ability class
 */
UCLASS()
class BLADERUSH_API UWeaponFireAbility : public UBaseWeaponAbility
{
	GENERATED_BODY()
public:
	UWeaponFireAbility();
	
	virtual bool CanFire(const UWeaponItemInstance* WeaponInstance, const UInventoryItemInstance* InventoryItemInstance, const ABaseCharacter* Character,const APlayerController* PlayerController) const;
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	virtual void ActivateAbilityWithTargetData(const FGameplayAbilityTargetDataHandle& TargetDataHandle, FGameplayTag ApplicationTag);
	virtual void ActivateLocalPlayerAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);
	
	virtual void WeaponFire(const FGameplayAbilityActorInfo* ActorInfo,const FWeaponTraceData& StartWeaponTraceData, UWeaponItemInstance* WeaponInstance,OUT TArray<FHitResult> Impacts);

	void ApplyDamageToTarget(const FHitResult& Hit, const UWeaponItemInstance* WeaponInstance);
	
	FHitResult SingleBulletFire(const FWeaponTraceData& StartWeaponTraceData, UWeaponItemInstance* WeaponInstance,OUT TArray<FHitResult> Impacts);
	FWeaponTraceData CalculateStartWeaponTraceData(ABaseCharacter* Character,APlayerController* PlayerController, UWeaponItemInstance* WeaponInstance) const;

	virtual void NotifyTargetDataReady(const FGameplayAbilityTargetDataHandle& InData, FGameplayTag ApplicationTag);
	

	UPROPERTY(EditDefaultsOnly)
	FShooterGameplayEffect FireEffect;

	UPROPERTY(EditDefaultsOnly)
	FShooterGameplayEffect DamageEffect;
	
private:
	FDelegateHandle NotifyTargetDataReadyDelegateHandle;
};
