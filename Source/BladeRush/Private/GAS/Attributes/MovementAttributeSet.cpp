// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Attributes/MovementAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

void UMovementAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, GrapplingProjectileMaxDistance, COND_None, REPNOTIFY_Always);

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UMovementAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
}

void UMovementAttributeSet::OnRep_GrapplingProjectileMaxDistance(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, GrapplingProjectileMaxDistance, OldValue);
}
