// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Attributes/AttributeHitPoints.h"
#include "Net/UnrealNetwork.h"
#include "Delegates/Delegate.h"
#include "GameplayEffectExtension.h"
#include "Characters/BaseCharacter.h"

UAttributeHitPoints::UAttributeHitPoints()
{
	HitPoints = 100.f;
	MaxHitPoints = 100.f;
	bOutOfHitPoints = false;
}

void UAttributeHitPoints::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, HitPoints, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxHitPoints, COND_None, REPNOTIFY_Always);
}

void UAttributeHitPoints::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHitPointsAttribute())
	{

	}
	
	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();
	const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();
	
	FGameplayTagContainer SpecDynamicAssetTags = Data.EffectSpec.DynamicAssetTags;
	FGameplayTagContainer SpecAssetTags;	
	Data.EffectSpec.GetAllAssetTags(SpecAssetTags);

	// Get the Target actor
	AActor* TargetActor = nullptr;
	AController* TargetController = nullptr;
	ABaseCharacter* TargetCharacter = nullptr;
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		TargetCharacter = Cast<ABaseCharacter>(TargetActor);
	}

	// Get the Source actor
	AActor* SourceActor = nullptr;
	AController* SourceController = nullptr;
	ABaseCharacter* SourceCharacter = nullptr;
	if (Source && Source->AbilityActorInfo.IsValid() && Source->AbilityActorInfo->AvatarActor.IsValid())
	{
		SourceActor = Source->AbilityActorInfo->AvatarActor.Get();
		SourceController = Source->AbilityActorInfo->PlayerController.Get();
		if (SourceController == nullptr && SourceActor != nullptr)
		{
			if (APawn* Pawn = Cast<APawn>(SourceActor))
			{
				SourceController = Pawn->GetController();
			}
		}

		// Use the controller to find the source pawn
		if (SourceController)
		{
			SourceCharacter = Cast<ABaseCharacter>(SourceController->GetPawn());
		}
		else
		{
			SourceCharacter = Cast<ABaseCharacter>(SourceActor);
		}

		// Set the causer actor based on context if it's set
		if (Context.GetEffectCauser())
		{
			SourceActor = Context.GetEffectCauser();
		}
	}

	if (Data.EvaluatedData.Attribute == GetHitDamageAttribute())
	{
		FHitResult HitResult;
		if (Context.GetHitResult())
		{
			HitResult = *Context.GetHitResult();
		}

		const float LocalDamageDone = GetHitDamage();
		SetHitDamage(0.f);

		// set new health
		const float NewHealth = GetHitPoints() - LocalDamageDone;
		SetHitPoints(FMath::Clamp(NewHealth, 0.0f, GetMaxHitPoints()));

		if (GetHitPoints() <= 0.0f)
		{
			bOutOfHitPoints = true;
			
			const FGameplayEffectContextHandle& EffectContext = Data.EffectSpec.GetEffectContext();
			AActor* Instigator = EffectContext.GetOriginalInstigator();
			AActor* Causer = EffectContext.GetEffectCauser();

			OnOutOfHitPoints.Broadcast(Instigator, Causer, Data.EffectSpec, Data.EvaluatedData.Magnitude);
		}
		
		if (TargetActor)
		{
			if (HitDamageReceivedDelegate.IsBound())
			{
				HitDamageReceivedDelegate.Broadcast(HitResult,SourceTags,Data.EffectSpec.GetContext());
			}
		}
	}
}

void UAttributeHitPoints::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void UAttributeHitPoints::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void UAttributeHitPoints::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
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
	if (bOutOfHitPoints && (GetHitPoints() > 0.0f))
	{
		bOutOfHitPoints = false;
	}
}

void UAttributeHitPoints::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetHitPointsAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHitPoints());
	}
}

void UAttributeHitPoints::OnRep_HitPoints(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, HitPoints, OldValue);
}

void UAttributeHitPoints::OnRep_MaxHitPoints(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxHitPoints, OldValue);
}
