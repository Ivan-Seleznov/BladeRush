// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/Movement/SlidePlayerAbility.h"

#include "Characters/BaseCharacter.h"
#include "Characters/Components/ShooterMovementComponent.h"

void USlidePlayerAbility::InputReleased(const FGameplayAbilitySpecHandle Handle,
                                        const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
}

bool USlidePlayerAbility::CanActivateMovementAbility(ABaseCharacter* Character,
	UShooterMovementComponent* ShooterMovementComponent) const
{
	return ShooterMovementComponent->Safe_bWantsToSprint && ShooterMovementComponent->CanSlide();
}

void USlidePlayerAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	ABaseCharacter* Character = GetCharacterFromActorInfo();
	if (!Character) return;

	UShooterMovementComponent* MovementComponent = Character->GetShooterMovementComponent();
	if (!MovementComponent) return;

}

void USlidePlayerAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
