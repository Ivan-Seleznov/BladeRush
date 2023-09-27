// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Attributes/AttributeStamina.h"
#include "Net/UnrealNetwork.h"
#include "Delegates/Delegate.h"
#include "GameplayEffectExtension.h"

UAttributeStamina::UAttributeStamina()
{
	// StaminaPoints = 100.f;
	// MaxStaminaPoints = 100.f;
	bOutOfStaminaPoints = false;
}

void UAttributeStamina::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, StaminaPoints, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxStaminaPoints, COND_None, REPNOTIFY_Always);
}

void UAttributeStamina::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetStaminaPointsAttribute())
	{
		if (GetStaminaPoints() <= 0 && bOutOfStaminaPoints)
		{
			if (OnOutOfStaminaPoints.IsBound())
			{
				const FGameplayEffectContextHandle& EffectContext = Data.EffectSpec.GetEffectContext();
				AActor* Instigator = EffectContext.GetOriginalInstigator();
				AActor* Causer = EffectContext.GetEffectCauser();

				OnOutOfStaminaPoints.Broadcast(Instigator, Causer, Data.EffectSpec, Data.EvaluatedData.Magnitude);
			}
		}
		bOutOfStaminaPoints = GetStaminaPoints() <= 0.0f;
	}
}

void UAttributeStamina::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void UAttributeStamina::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void UAttributeStamina::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	if (bOutOfStaminaPoints && (GetStaminaPoints() > 0.0f))
	{
		bOutOfStaminaPoints = false;
	}
}

void UAttributeStamina::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetStaminaPointsAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxStaminaPoints());
	}
}

void UAttributeStamina::OnRep_StaminaPoints(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, StaminaPoints, OldValue);
}

void UAttributeStamina::OnRep_MaxStaminaPoints(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxStaminaPoints, OldValue);
}
