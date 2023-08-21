// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/Movement/MovementAbilityBase.h"
#include "CrouchPlayerAbility.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API UCrouchPlayerAbility : public UMovementAbilityBase
{
	GENERATED_BODY()
public:
	UCrouchPlayerAbility();
	
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual bool CanActivateMovementAbility(ABaseCharacter* Character, UShooterMovementComponent* ShooterMovementComponent) const override;
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION()
	void MovementModeChanged(ACharacter* Character,EMovementMode PrevMovementMode, uint8 PreviousCustomMode);
private:
	bool bCrouching;
};
