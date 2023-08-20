// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GAS/ShooterAbilitySet.h"
#include "BaseCharacter.generated.h"

class UShooterAbilitySet;
class UPlayerAbilitySystemComponent;
class UAbilitySystemComponent;
class UAttributeHealth;
class UGameplayEffect;
class UGameplayAbility;
class UShooterCharacterData;

UCLASS()
class SIMPLESHOOTER_API ABaseCharacter : public ACharacter,public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable)
	void TryApplyAbilitySet(const UShooterAbilitySet* AbilitySet, bool bCancelEarlySet = false);
	
protected:

	UFUNCTION(Server, Unreliable)
	void TryApplyAbilitySet_Server(const UShooterAbilitySet* AbilitySet, bool bCancelEarlySet = false);
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="GAS")
	TObjectPtr<UPlayerAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|AbiliySet")
	TObjectPtr<const UShooterAbilitySet> DefaultAbilitySet;

	// Authority-only list of granted handles
	UPROPERTY(VisibleAnywhere)
	FAbilitySet_GrantedHandles GrantedHandles;
	
	virtual void BeginPlay() override;

};
