// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/GameplayAbilityBase.h"
#include "MovementAbilityBase.generated.h"

class UShooterMovementComponent;

/**
 * 
 */
UCLASS(Abstract)
class BLADERUSH_API UMovementAbilityBase : public UGameplayAbilityBase
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
							 const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

	virtual bool CanActivateMovementAbility(ABaseCharacter* Character,UShooterMovementComponent* ShooterMovementComponent) const;
	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
protected:
	UPROPERTY(EditAnywhere,Category="GameplayEffects")
	FShooterGameplayEffect DebuffEffect;

private:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="GameplayEffects", meta=(AllowPrivateAccess="true"))
	FShooterGameplayEffect AbilityTagEffect;

	UPROPERTY()
	FActiveGameplayEffectHandle ActiveAbilityTagEffectHandle;
};
