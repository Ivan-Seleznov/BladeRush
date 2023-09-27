// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\..\..\Public\GAS\Abilities\Movement\CrouchSlidePlayerAbility.h"

#include "AbilitySystemComponent.h"
#include "Characters/BaseCharacter.h"
#include "Characters/Components/ShooterMovementComponent.h"
#include "GAS/Attributes/AttributeStamina.h"


UCrouchSlidePlayerAbility::UCrouchSlidePlayerAbility()
{
	bCrouching = false;
}

void UCrouchSlidePlayerAbility::InputPressed(const FGameplayAbilitySpecHandle Handle,
                                        const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	ABaseCharacter* Character = GetCharacterFromActorInfo();
	if (!Character) return;
	
	if (bCrouching)
	{
		EndAbility(Handle,ActorInfo,ActivationInfo,true,true);
		return;
	}

	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
}

bool UCrouchSlidePlayerAbility::CanActivateMovementAbility(ABaseCharacter* Character,
	UShooterMovementComponent* ShooterMovementComponent) const
{
	return Character->CanCrouch();
}

void UCrouchSlidePlayerAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ABaseCharacter* Character = GetCharacterFromActorInfo();
	if (!Character) return;

	UShooterMovementComponent* MovementComponent = Character->GetShooterMovementComponent();
	if (!MovementComponent) return;

	Character->MovementModeChangedDelegate.AddDynamic(this,&ThisClass::MovementModeChanged);


	const UAttributeStamina* AttributeStamina = UAttributeStamina::Find(Character->GetAbilitySystemComponent());
	
	if (MovementComponent->CanSlide() && AttributeStamina->GetStaminaPoints() > AttributeStamina->GetMaxStaminaPoints() * BlockSlideMultiplier)
	{
		ActivateSlide(Character,MovementComponent);
	}
	else
	{
		ActivateCrouch(Character,MovementComponent);
	}
	
	bCrouching = true;
}

void UCrouchSlidePlayerAbility::ActivateSlide(ABaseCharacter* Character,
	UShooterMovementComponent* ShooterMovementComponent)
{
	Character->StartSliding();
	
	const FGameplayEffectContextHandle GameplayEffectContextHandle;
	Character->GetAbilitySystemComponent()->ApplyGameplayEffectToSelf(SlideEffect.GameplayEffect.GetDefaultObject(), SlideEffect.Level, GameplayEffectContextHandle);
}

void UCrouchSlidePlayerAbility::ActivateCrouch(ABaseCharacter* Character,
	UShooterMovementComponent* ShooterMovementComponent)
{
	Character->Crouch();
}

void UCrouchSlidePlayerAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
	ABaseCharacter* Character = GetCharacterFromActorInfo();
	if (!Character) return;

	if (!SlideEffect.GameplayEffect) return;
	Character->GetAbilitySystemComponent()->RemoveActiveGameplayEffectBySourceEffect(SlideEffect.GameplayEffect, nullptr);
	
	Character->UnCrouch();
	
}

void UCrouchSlidePlayerAbility::MovementModeChanged(ACharacter* Character, EMovementMode PrevMovementMode,
	uint8 PreviousCustomMode)
{
	if (PrevMovementMode == CMOVE_Slide) EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
