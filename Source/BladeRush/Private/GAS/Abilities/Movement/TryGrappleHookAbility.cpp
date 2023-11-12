// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/Movement/TryGrappleHookAbility.h"

#include "Characters/BaseCharacter.h"
#include "Characters/Components/ShooterMovementComponent.h"
#include "GAS/Abilities/Movement/JumpPlayerAbility.h"


void UTryGrappleHookAbility::InputPressed(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
	
	ABaseCharacter* Character = GetCharacterFromActorInfo();
	if (!Character) return;
	
	UShooterMovementComponent* MovementComponent = Character->GetShooterMovementComponent();
	if (!MovementComponent) return;
	
	if (IsActive())
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

	if (Character->IsLocallyControlled())
	{
		MovementComponent->TryGrapple();
	}
	
	Character->MovementModeChangedDelegate.AddDynamic(this,&ThisClass::OnMovementModeChanged);
	MovementComponent->OnGrappleFailed.AddUObject(this,&ThisClass::OnGrappleFailed);
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
	MovementComponent->OnGrappleFailed.RemoveAll(this);
}

void UTryGrappleHookAbility::OnMovementModeChanged(ACharacter* Character, EMovementMode PrevMovementMode,
	uint8 PreviousCustomMode)
{
	if (PrevMovementMode == MOVE_Custom && PreviousCustomMode == CMOVE_Grappling)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}

void UTryGrappleHookAbility::OnGrappleFailed(AActor* ProjectileOwner)
{
	if (ProjectileOwner->HasAuthority())
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}

