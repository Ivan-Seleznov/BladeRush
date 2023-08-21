// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/Movement/MovementAbilityBase.h"
#include "SprintPlayerAbility.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API USprintPlayerAbility : public UMovementAbilityBase
{
	GENERATED_BODY()
public:
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	virtual bool CanActivateMovementAbility(ABaseCharacter* Character, UShooterMovementComponent* ShooterMovementComponent) const override;
	//virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;	

protected:
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
							 const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION()
	void OnMovementChanged(float DeltaSeconds, FVector OldLocation, FVector OldVelocity);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FShooterGameplayEffect SprintEffect;
	
	//UPROPERTY(Replicated)
	//ABaseCharacter* PlayerCharacter;
};
