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
		//default abilities
		for(const FCharacterAbilities& TakedAbility : DefaultAbilities)
		{
			FGameplayAbilitySpec GameplayAbilitySpec(TakedAbility.Ability,TakedAbility.Level,static_cast<int32>(TakedAbility.InputID), TakedAbility.InSourceObject);
			GameplayAbilitySpec.DynamicAbilityTags.AddTag(TakedAbility.InputTag);
			
			AbilitySystemComponent->GiveAbility(GameplayAbilitySpec);

			if (TakedAbility.bAutoActivate)
			{
				AbilitySystemComponent->TryActivateAbilityByClass(TakedAbility.Ability);
				
			}
		}
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

const UShooterCharacterData* ABaseCharacter::GetCharacterData()
{
	return CharacterData.Get();
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
