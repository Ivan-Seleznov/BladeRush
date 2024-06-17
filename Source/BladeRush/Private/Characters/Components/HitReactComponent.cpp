// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Components/HitReactComponent.h"

#include "Characters/BaseCharacter.h"
#include "GAS/PlayerAbilitySystemComponent.h"
#include "GAS/Attributes/AttributeHitPoints.h"



UHitReactComponent::UHitReactComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UHitReactComponent::OnHitReactClient(const FHitResult& HitResult)
{
	K2_OnHitReact(HitResult,GetHitDirection(HitResult));
}

void UHitReactComponent::BeginPlay()
{
	Super::BeginPlay();

	const ABaseCharacter* OwnerCharacter = GetPawn<ABaseCharacter>();
	if (!OwnerCharacter) return;

	const UPlayerAbilitySystemComponent* AbilitySystemComponent = Cast<UPlayerAbilitySystemComponent>(OwnerCharacter->GetAbilitySystemComponent());
	if (!AbilitySystemComponent)
	{
		return;
	}
	
	const UAttributeHitPoints* HealthAttributeSet = UAttributeHitPoints::Find(AbilitySystemComponent);
	if (!HealthAttributeSet)
	{
		return;
	}

	HealthAttributeSet->HitDamageReceivedDelegate.AddDynamic(this,&ThisClass::OnHitReact);
}

void UHitReactComponent::OnHitReact(FHitResult HitResult, FGameplayTagContainer SourceTags,
	FGameplayEffectContextHandle GameplayEffectSpec)
{
	if (!HasAuthority())
	{
		return;
	}
	
	Multicast_OnHitReact(HitResult,GetHitDirection(HitResult));
}

FVector UHitReactComponent::GetHitDirection(const FHitResult& HitResult) const
{
	return (HitResult.Location - HitResult.TraceStart).GetSafeNormal();
}

void UHitReactComponent::Multicast_OnHitReact_Implementation(FHitResult HitResult, FVector HitDirection)
{
	K2_OnHitReact(HitResult,HitDirection);	
}

