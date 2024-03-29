// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/Movement/SprintPlayerAbility.h"

#include "AbilitySystemComponent.h"
#include "BladeRushLogs.h"
#include "Characters/BaseCharacter.h"
#include "Characters/Components/ShooterMovementComponent.h"
#include "GAS/Attributes/AttributeStamina.h"

void USprintPlayerAbility::InputReleased(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
	EndAbility(Handle, ActorInfo, ActivationInfo,true,true);
}

bool USprintPlayerAbility::CanActivateMovementAbility(ABaseCharacter* Character,
	UShooterMovementComponent* ShooterMovementComponent) const
{
	//const UAttributeStamina* AttributeStamina = UAttributeStamina::Find(Character->GetAbilitySystemComponent());

	//const float StaminaPoints = AttributeStamina->GetStaminaPoints();
	//const float MaxStaminaPoints = AttributeStamina->GetMaxStaminaPoints();
	
	return ShooterMovementComponent->CanSprint();
}

void USprintPlayerAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ABaseCharacter* Character = GetCharacterFromActorInfo();
	if (!Character)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
		return;
	}

	const UShooterMovementComponent* MovementComponent = Character->GetShooterMovementComponent();
	if (!MovementComponent)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
		return;
	}

	if (Character->IsLocallyControlled())
	{
		Character->StartSprinting();
		DEBUG_LOG("Activate Sprint");
	}

	const FGameplayEffectContextHandle GameplayEffectContextHandle;
	Character->GetAbilitySystemComponent()->ApplyGameplayEffectToSelf(SprintEffect.GameplayEffect.GetDefaultObject(), SprintEffect.Level, GameplayEffectContextHandle);
		
	Character->OnCharacterMovementUpdated.AddDynamic(this, &ThisClass::OnMovementChanged);
}
	
void USprintPlayerAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	ABaseCharacter* Character = GetCharacterFromActorInfo();
	
	Character->OnCharacterMovementUpdated.RemoveAll(this);
	if (Character->IsLocallyControlled())
	{
		Character->StopSprinting();
	}
		
	if (SprintEffect.GameplayEffect)
	{
		Character->GetAbilitySystemComponent()->RemoveActiveGameplayEffectBySourceEffect(SprintEffect.GameplayEffect, nullptr);
	}
}

void USprintPlayerAbility::OnMovementChanged(float DeltaSeconds, FVector OldLocation, FVector OldVelocity)
{
	ABaseCharacter* Character = GetCharacterFromActorInfo();
	check(Character);
	
	UShooterMovementComponent* MovementComponent = Character->GetShooterMovementComponent();
	check(MovementComponent);
	
	if (!CanActivateMovementAbility(Character,MovementComponent))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}