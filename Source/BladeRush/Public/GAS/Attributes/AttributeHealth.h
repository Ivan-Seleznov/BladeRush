// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Attributes/AttributeBase.h"
#include "AttributeHealth.generated.h"

/**
 * 
 */
UCLASS()
class BLADERUSH_API UAttributeHealth : public UAttributeBase
{
	GENERATED_BODY()
public:
	UAttributeHealth();

	static const UAttributeHealth* Find(const UAbilitySystemComponent* AbilitySystemComponent)
	{
		if (!AbilitySystemComponent) return nullptr;

		TSubclassOf<UAttributeHealth> HealthPoints = UAttributeHealth::StaticClass();
		return Cast<UAttributeHealth>(AbilitySystemComponent->GetAttributeSet(HealthPoints));
	};
	
	mutable FAttributeEvent OnOutOfHealthPoints;
	
	ATTRIBUTE_ACCESSORS(UAttributeHealth, HealthPoints);
	ATTRIBUTE_ACCESSORS(UAttributeHealth, MaxHealthPoints);
	void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	virtual void ClampAttribute(const FGameplayAttribute& Attribute,float& NewValue) const;
	
	UFUNCTION()
	void OnRep_HealthPoints(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_MaxHealthPoints(const FGameplayAttributeData& OldValue);
private:
	UPROPERTY(EditAnywhere,BlueprintReadOnly, ReplicatedUsing = OnRep_HealthPoints, Category = "Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData HealthPoints;
	UPROPERTY(EditAnywhere,BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealthPoints, Category = "Healath", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealthPoints;

	bool bOutOfHealthPoints;
};
