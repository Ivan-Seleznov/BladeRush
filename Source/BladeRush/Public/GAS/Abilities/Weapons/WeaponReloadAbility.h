// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWeaponAbility.h"
#include "WeaponReloadAbility.generated.h"

class UMagazineItemInstance;
/**
 * 
 */
UCLASS()
class BLADERUSH_API UWeaponReloadAbility : public UBaseWeaponAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION(BlueprintImplementableEvent)
	void K2_PlayReloadAnimation();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	FShooterGameplayEffect ReloadEffect;

	FActiveGameplayEffectHandle ReloadEffectActiveHandle;
	
	UFUNCTION(BlueprintCallable)
	void OnWeaponReloaded();	

	UFUNCTION(BlueprintPure)
	UMagazineItemInstance* GetMagazineItemInstance() const;
};
