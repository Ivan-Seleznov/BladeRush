// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"

#include "Characters/Components/ShooterHeroComponent.h"
#include "GAS/PlayerAbilitySystemComponent.h"
#include "GAS/Attributes/AttributeHealth.h"

ABaseCharacter::ABaseCharacter()
{
	AbilitySystemComponent = CreateDefaultSubobject<UPlayerAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

}

void ABaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	if (!AbilitySystemComponent) return;
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	if (HasAuthority())
	{
		TryApplyAbilitySet(DefaultAbilitySet);
	}
}

void ABaseCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	//client gas init

	if (!AbilitySystemComponent) return;
	
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
}

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ABaseCharacter::TryApplyAbilitySet(const UShooterAbilitySet* AbilitySet, bool bCancelEarlySet)
{
	// Clear all
	if (bCancelEarlySet)
	{
		//AbilitySystemComponent->ClearAllAbilities();
		//AbilitySystemComponent->RemoveAllSpawnedAttributes();

		GrantedHandles.TakeFromAbilitySystem(AbilitySystemComponent);
		
		//FGameplayEffectQuery Query;
		//AbilitySystemComponent->RemoveActiveEffects(Query);
	}

	if (!HasAuthority())
	{
		TryApplyAbilitySet_Server(AbilitySet, bCancelEarlySet);
	}

	if (AbilitySet)
	{
		AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, &GrantedHandles);
	}
}

void ABaseCharacter::TryApplyAbilitySet_Server_Implementation(const UShooterAbilitySet* AbilitySet,
	bool bCancelEarlySet)
{
	// Clear all
	if (bCancelEarlySet)
	{
		//GetAbilitySystemComponent()->ClearAllAbilities();
		//GetAbilitySystemComponent()->RemoveAllSpawnedAttributes();

		GrantedHandles.TakeFromAbilitySystem(AbilitySystemComponent);
		
		//FGameplayEffectQuery Query;
		//GetAbilitySystemComponent()->RemoveActiveEffects(Query);
	}
	
	if (AbilitySet)
	{
		AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, &GrantedHandles);
	}
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}
