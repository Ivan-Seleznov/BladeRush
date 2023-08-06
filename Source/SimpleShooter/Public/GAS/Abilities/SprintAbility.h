// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/GameplayAbilityBase.h"
#include "SprintAbility.generated.h"

/**
 * 
 */
class ABaseCharacter;

UCLASS()
class SIMPLESHOOTER_API USprintAbility : public UGameplayAbilityBase
{
	GENERATED_BODY()
	
public:
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo) override;
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
							 const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	UFUNCTION()
	void OnMovementChanged(float DeltaSeconds, FVector OldLocation, FVector OldVelocity);
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta = (AllowPrivateAccess = true))
	float StaminaBlocked = 15.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta = (AllowPrivateAccess = true))
	FGameGameplayEffect SprintEffect;

	ABaseCharacter* Character;
};
