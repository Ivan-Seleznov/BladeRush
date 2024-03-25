// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/Weapons/BaseWeaponAbility.h"
#include "ADSAbility.generated.h"

class UCameraMode_InterpLocation;
class ABladeRushCameraManager;
/**
 * ADS gameplay ability
 */
UCLASS()
class BLADERUSH_API UADSAbility : public UBaseWeaponAbility
{
	GENERATED_BODY()
public:
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
protected:
	UFUNCTION(BlueprintCallable)
	ABladeRushCameraManager* GetCameraManager() const;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FShooterGameplayEffect ADSGameplayEffect;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)	
	TSubclassOf<UCameraMode_InterpLocation> CameraModeClass;
	
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle;
};
