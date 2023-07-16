// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "BaseCharacter.generated.h"

class UPlayerAbilitySystemComponent;
class UAbilitySystemComponent;
class UAttributeHealth;
class UGameplayEffect;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay|GAS")
	TObjectPtr<UAttributeHealth> HealthPoints;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay|GAS")
	TArray<TSubclassOf<UGameplayEffect>> DefaultEffects;

	virtual void InitializeAttributes() const;
	virtual void BeginPlay() override;
};
