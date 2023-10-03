// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/Movement/TryGrappleHookAbility.h"

#include "Characters/BaseCharacter.h"
#include "Characters/Components/ShooterMovementComponent.h"


void UTryGrappleHookAbility::InputPressed(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
	
	ABaseCharacter* Character = GetCharacterFromActorInfo();
	if (!Character) return;
	
	UShooterMovementComponent* MovementComponent = Character->GetShooterMovementComponent();
	if (!MovementComponent) return;
	
	if (MovementComponent->IsGrappling())
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}

void UTryGrappleHookAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                             const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                             const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	ABaseCharacter* Character = GetCharacterFromActorInfo();
	if (!Character) return;
	
	UShooterMovementComponent* MovementComponent = Character->GetShooterMovementComponent();
	if (!MovementComponent) return;

	if (!MovementComponent->TryGrapple())
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}
	
	Character->MovementModeChangedDelegate.AddDynamic(this,&ThisClass::OnMovementModeChanged);
}

void UTryGrappleHookAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
	ABaseCharacter* Character = GetCharacterFromActorInfo();
	if (!Character) return;

	UShooterMovementComponent* MovementComponent = Character->GetShooterMovementComponent();
	if (!MovementComponent) return;

	MovementComponent->StopGrappling();
	
	Character->MovementModeChangedDelegate.RemoveAll(this);
}

void UTryGrappleHookAbility::OnMovementModeChanged(ACharacter* Character, EMovementMode PrevMovementMode,
	uint8 PreviousCustomMode)
{
	if (PrevMovementMode == MOVE_Custom && PreviousCustomMode == CMOVE_Grappling)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		GEngine->AddOnScreenDebugMessage(-1,2,FColor::Blue,"on movement mode changed end grapple ability");
	}
}

