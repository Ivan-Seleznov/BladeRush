// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Attributes/AttributeHealth.h"
#include "Net/UnrealNetwork.h"
#include "Delegates/Delegate.h"
#include "GameplayEffectExtension.h"

UAttributeHealth::UAttributeHealth()
{
	// HealthPoints = 100.f;
	// MaxHealthPoints = 100.f;
	bOutOfHealthPoints = false;
}

void UAttributeHealth::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, HealthPoints, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxHealthPoints, COND_None, REPNOTIFY_Always);
}

void UAttributeHealth::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthPointsAttribute())
	{
		if (GetHealthPoints() <= 0 && bOutOfHealthPoints)
		{
			if (OnOutOfHealthPoints.IsBound())
			{
				const FGameplayEffectContextHandle& EffectContext = Data.EffectSpec.GetEffectContext();
				AActor* Instigator = EffectContext.GetOriginalInstigator();
				AActor* Causer = EffectContext.GetEffectCauser();

				OnOutOfHealthPoints.Broadcast(Instigator, Causer, Data.EffectSpec, Data.EvaluatedData.Magnitude);
			}
		}
		bOutOfHealthPoints = GetHealthPoints() <= 0.0f;
	}
}

void UAttributeHealth::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void UAttributeHealth::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void UAttributeHealth::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	
	// if (Attribute == GetMaxHitPointsAttribute())
	// {
	// 	if (GetHitPoints() > NewValue)
	// 	{
	// 		UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponent<UAbilitySystemComponent>();
	// 		check(AbilitySystemComponent);
	//
	// 		AbilitySystemComponent->ApplyModToAttribute(GetHitPointsAttribute(), EGameplayModOp::Override, NewValue);
	// 	}
	// }
	if (bOutOfHealthPoints && (GetHealthPoints() > 0.0f))
	{
		bOutOfHealthPoints = false;
	}
}

void UAttributeHealth::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetHealthPointsAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealthPoints());
	}
}

void UAttributeHealth::OnRep_HealthPoints(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, HealthPoints, OldValue);
}

void UAttributeHealth::OnRep_MaxHealthPoints(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxHealthPoints, OldValue);
}
