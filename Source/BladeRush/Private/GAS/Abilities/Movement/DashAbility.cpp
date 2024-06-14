// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/Movement/DashAbility.h"

#include "BladeRushLogs.h"
#include "Characters/BaseCharacter.h"
#include "Characters/Components/ShooterMovementComponent.h"

bool UDashAbility::CanActivateMovementAbility(ABaseCharacter* Character,
    UShooterMovementComponent* ShooterMovementComponent) const
{
    return Super::CanActivateMovementAbility(Character, ShooterMovementComponent) && ShooterMovementComponent->CanDash();
}

void UDashAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    UShooterMovementComponent* ShooterMovementComponent = GetShooterMovementComponent();
    if (!ShooterMovementComponent)
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
        return;
    }

    ShooterMovementComponent->MovementActionEndedDelegate.AddDynamic(this,&ThisClass::OnMovementActionEnded);
    
    if (ShooterMovementComponent->IsCharacterOwnerLocallyControlled())
    {
        ShooterMovementComponent->Dash();
    }
}

void UDashAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

    DEBUG_LOG("DashAbility ended");

    ApplyCooldown(Handle,ActorInfo,ActivationInfo);
    if (UShooterMovementComponent* ShooterMovementComponent = GetShooterMovementComponent())
    {
        ShooterMovementComponent->MovementActionEndedDelegate.RemoveAll(this);
    }
}

void UDashAbility::OnMovementActionEnded(TEnumAsByte<EMovementAction> EndedMovementAction)
{
    if (EndedMovementAction != EMovementAction::Dash)
    {
        return;
    }

	EndAbility(GetCurrentAbilitySpecHandle(),GetCurrentActorInfo(),GetCurrentActivationInfo(),false,false);
}