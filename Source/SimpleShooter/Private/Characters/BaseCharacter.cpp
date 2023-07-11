// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"

#include "GAS/PlayerAbilitySystemComponent.h"
#include "GAS/Attributes/AttributeHealth.h"

ABaseCharacter::ABaseCharacter()
{
	AbilitySystemComponent = CreateDefaultSubobject<UPlayerAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	HealthPoints = CreateDefaultSubobject<UAttributeHealth>("UAttributeHealth");
}

void ABaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	//server gas init
	if (HasAuthority())
	{
		//set owner
		AbilitySystemComponent->InitAbilityActorInfo(this,this);
		InitializeAttributes();
	}
}

void ABaseCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	//client gas init

	if (!AbilitySystemComponent) return;
	
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	InitializeAttributes();
}

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ABaseCharacter::InitializeAttributes() const
{
	if (AbilitySystemComponent && !DefaultEffects.IsEmpty())
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		for (TSubclassOf<UGameplayEffect> Effect : DefaultEffects)
		{
			AbilitySystemComponent->ApplyGameplayEffectToSelf(Effect.GetDefaultObject(),1.f,EffectContext);
		}
	}
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}
