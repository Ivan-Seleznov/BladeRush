// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Attributes/MovementAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

void UMovementAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, GrapplingHookLength, COND_None, REPNOTIFY_Always);
	
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UMovementAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	
	/*ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(GetOwningActor());
	if (!BaseCharacter) return;

	UShooterMovementComponent* MovementComponent = BaseCharacter->GetShooterMovementComponent();
	if (!MovementComponent) return;*/
}


void UMovementAttributeSet::OnRep_GrapplingHookLength(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, GrapplingHookLength, OldValue);
}

void UMovementAttributeSet::OnRep_GrapplingProjectileMaxDistance(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, GrapplingProjectileMaxDistance, OldValue);
}
