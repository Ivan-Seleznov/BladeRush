// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Attributes/AttributeHitPoints.h"
#include "Net/UnrealNetwork.h"
#include "Delegates/Delegate.h"
#include "GameplayEffectExtension.h"
#include "Characters/BaseCharacter.h"
#include "Characters/Player/PlayerCharacter.h"
#include "GameMods/BladeRushGameState.h"
#include "GameMods/BladeRushPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Weapons/BaseWeaponActor.h"

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

	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();
	const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();
	
	const FGameplayTagContainer& SpecDynamicAssetTags = Data.EffectSpec.GetDynamicAssetTags();
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
			const FGameplayEffectContextHandle& EffectContext = Data.EffectSpec.GetEffectContext();
			ABaseCharacter* Instigator = Cast<ABaseCharacter>(EffectContext.GetOriginalInstigator());
			AActor* Causer = EffectContext.GetEffectCauser();
		} 
		
		if (TargetActor)
		{
			if (HitDamageReceivedDelegate.IsBound())
			{
				HitDamageReceivedDelegate.Broadcast(HitResult,SourceTags,Data.EffectSpec.GetContext());
			}
		}
	}

	if (bOutOfHitPoints && !bPrevOutOfHitPoints)
	{
		if (SourceController && SourceController != TargetController)
		{
			if (ABladeRushPlayerState* SourcePlayerState = SourceController->GetPlayerState<ABladeRushPlayerState>())
			{
				SourcePlayerState->AddKill();
			}
		}
		if (TargetController)
		{
			if (ABladeRushPlayerState* TargetPlayerState = TargetController->GetPlayerState<ABladeRushPlayerState>())
			{
				TargetPlayerState->AddDeath();
			}
		}
		if (SourceController && TargetController)
		{
			ABladeRushGameState* BladeRushGameState = Cast<ABladeRushGameState>(UGameplayStatics::GetGameState(this));
			ABladeRushPlayerState* TargetPlayerState = TargetController->GetPlayerState<ABladeRushPlayerState>();
			ABladeRushPlayerState* SourcePlayerState = SourceController->GetPlayerState<ABladeRushPlayerState>();
			
			const FGameplayEffectContextHandle& EffectContext = Data.EffectSpec.GetEffectContext();
			AActor* Causer = EffectContext.GetEffectCauser();
			
			if (BladeRushGameState && SourcePlayerState && TargetPlayerState && Causer)
			{
				BladeRushGameState->NotifyPlayerDeath(FDeadPlayerInfo(TargetPlayerState->GetPlayerName(),
					SourcePlayerState->GetPlayerName(), Causer));
			}
		}
	}
}

void UAttributeHitPoints::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

}

void UAttributeHitPoints::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

}

void UAttributeHitPoints::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	ClampAttribute(Attribute,NewValue);

	if (Attribute == GetHitPointsAttribute())
	{
		bPrevOutOfHitPoints = bOutOfHitPoints;
		
		if (NewValue <= 0 )
		{
			bOutOfHitPoints = true;
			OutOfHitPointsDelegate.Broadcast(OldValue);
		}
		if (bOutOfHitPoints && NewValue > 0.f)
		{
			bOutOfHitPoints = false;
		}
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