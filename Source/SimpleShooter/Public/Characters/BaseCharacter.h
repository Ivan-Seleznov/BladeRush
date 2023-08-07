// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "SimpleShooter.h"
#include "BaseCharacter.generated.h"

class UPlayerAbilitySystemComponent;
class UAbilitySystemComponent;
class UAttributeHealth;
class UGameplayEffect;
class UGameplayAbility;
class UShooterCharacterData;

USTRUCT(BlueprintType)
struct FCharacterAbilities
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayAbility> Ability;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGASAbilityInputID InputID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UObject* InSourceObject = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAutoActivate = false;

	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
};

UCLASS()
class SIMPLESHOOTER_API ABaseCharacter : public ACharacter,public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="GAS")
	TObjectPtr<UPlayerAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<UAttributeHealth> HealthPoints;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
	TArray<TSubclassOf<UGameplayEffect>> DefaultEffects;

	UPROPERTY(EditAnywhere,Category = "GAS")
	TArray<FCharacterAbilities> DefaultAbilities;
	
	
	virtual void InitializeAttributes() const;
	virtual void BeginPlay() override;
};
