// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Attributes/AttributeMovementSpeed.h"

#include "Characters/BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

void UAttributeMovementSpeed::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
		
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, WalkSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, RunSpeed, COND_None, REPNOTIFY_Always);
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UAttributeMovementSpeed::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	if (ABaseCharacter* Character = Cast<ABaseCharacter>(GetOwningActor()))
	{
		UE_LOG(LogTemp,Warning,TEXT("Changing player speed"));
		//Character->GetCharacterMovement()->MaxWalkSpeed = NewValue;
		
	}
}

void UAttributeMovementSpeed::OnRep_WalkSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, WalkSpeed, OldValue);
}

void UAttributeMovementSpeed::OnRep_RunSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, RunSpeed, OldValue);
}
