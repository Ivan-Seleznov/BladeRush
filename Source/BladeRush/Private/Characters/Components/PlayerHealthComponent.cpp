// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Components/PlayerHealthComponent.h"

#include "AbilitySystemComponent.h"
#include "Characters/BaseCharacter.h"
#include "GameMods/BladeRushGameMode.h"
#include "GAS/PlayerAbilitySystemComponent.h"
#include "GAS/Attributes/AttributeHealth.h"
#include "Kismet/GameplayStatics.h"

UPlayerHealthComponent::UPlayerHealthComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UPlayerHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
	const ABaseCharacter* OwnerCharacter = Cast<ABaseCharacter>(GetOwner());
	if (!OwnerCharacter) return;

	const UPlayerAbilitySystemComponent* AbilitySystemComponent = Cast<UPlayerAbilitySystemComponent>(OwnerCharacter->GetAbilitySystemComponent());
	if (!AbilitySystemComponent) return;
	
	HealthAttributeSet = UAttributeHealth::Find(AbilitySystemComponent);
	if (!HealthAttributeSet) return;

	
	HealthAttributeSet->OnOutOfHealthPoints.AddUObject(this,&ThisClass::HandleOutOfHealth);
}

float UPlayerHealthComponent::GetHealth() const
{
	return HealthAttributeSet ? HealthAttributeSet->GetHealthPoints() : 0.f;
}

void UPlayerHealthComponent::HandleOutOfHealth(AActor* DamageInstigator, AActor* DamageCauser,
                                               const FGameplayEffectSpec& DamageEffectSpec, float DamageMagnitude)
{
	ABaseCharacter* Character = Cast<ABaseCharacter>(DamageCauser);
	if (!Character) return;


	OnDeathFinished.Broadcast(GetOwner());
	CharacterDeath_Client(Character);
	
	ABladeRushGameMode* GameMode = Cast<ABladeRushGameMode>(UGameplayStatics::GetGameMode(Character));
	if (!GameMode) return;
	
	GameMode->CharacterDied(Character);
}

void UPlayerHealthComponent::CharacterDeath_Client_Implementation(ABaseCharacter* Character)
{
	OnDeathFinished.Broadcast(GetOwner());
}
