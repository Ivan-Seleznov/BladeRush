// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Components/PlayerHealthComponent.h"

#include "AbilitySystemComponent.h"
#include "Characters/BaseCharacter.h"
#include "GameMods/BladeRushGameMode.h"
#include "GAS/PlayerAbilitySystemComponent.h"
#include "GAS/Attributes/AttributeHitPoints.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

UPlayerHealthComponent::UPlayerHealthComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UPlayerHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass,bIsDead);
}

void UPlayerHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
	const ABaseCharacter* OwnerCharacter = Cast<ABaseCharacter>(GetOwner());
	check(OwnerCharacter);

	const UPlayerAbilitySystemComponent* AbilitySystemComponent = Cast<UPlayerAbilitySystemComponent>(OwnerCharacter->GetAbilitySystemComponent());
	if (!AbilitySystemComponent)
	{
		return;
	}
	
	HealthAttributeSet = UAttributeHitPoints::Find(AbilitySystemComponent);
	if (!HealthAttributeSet)
	{
		return;
	}

	HealthAttributeSet->OutOfHitPointsDelegate.AddUObject(this,&ThisClass::OnOutOfHealth);
	//OwnerCharacter->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(UAttributeHitPoints::GetHitPointsAttribute()).AddUObject(this, &ThisClass::OnHitPointsChanged);
}

float UPlayerHealthComponent::GetHealth() const
{
	return HealthAttributeSet ? HealthAttributeSet->GetHitPoints() : 0.f;
}

void UPlayerHealthComponent::OnOutOfHealth(const FDeadPlayerInfo& DeadPlayerInfo)
{
	ABaseCharacter* Character = Cast<ABaseCharacter>(GetOwner());
	check(Character);

	bIsDead = true;
	OnDeathFinished.Broadcast(GetOwner(), DeadPlayerInfo);
	CharacterDeath_Client(Character, DeadPlayerInfo);
	
	ABladeRushGameMode* GameMode = Cast<ABladeRushGameMode>(UGameplayStatics::GetGameMode(Character));
	if (!GameMode)
	{
		return;
	}
	
	GameMode->CharacterDied(Character);
}

void UPlayerHealthComponent::CharacterDeath_Client_Implementation(ABaseCharacter* Character, const FDeadPlayerInfo& DeadPlayerInfo)
{
	OnDeathFinished.Broadcast(GetOwner(), DeadPlayerInfo);
}
