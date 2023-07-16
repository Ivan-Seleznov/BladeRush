// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Attributes/AttributeBase.h"
#include "AttributeStamina.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API UAttributeStamina : public UAttributeBase
{
	GENERATED_BODY()
public:
	UAttributeStamina();

	static const UAttributeStamina* Find(const UAbilitySystemComponent* AbilitySystemComponent)
	{
		if (!AbilitySystemComponent) return nullptr;

		TSubclassOf<UAttributeStamina> StaminaPoints = UAttributeStamina::StaticClass();
		return Cast<UAttributeStamina>(AbilitySystemComponent->GetAttributeSet(StaminaPoints));
	};
	
	mutable FAttributeEvent OnOutOfStaminaPoints;
	
	ATTRIBUTE_ACCESSORS(UAttributeStamina, StaminaPoints);
	ATTRIBUTE_ACCESSORS(UAttributeStamina, MaxStaminaPoints);
	void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	virtual void ClampAttribute(const FGameplayAttribute& Attribute,float& NewValue) const;

	UFUNCTION()
	void OnRep_StaminaPoints(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	void OnRep_MaxStaminaPoints(const FGameplayAttributeData& OldValue);
private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StaminaPoints, Category = "Attributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData StaminaPoints;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StaminaPoints, Category = "Attributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxStaminaPoints;

	bool bOutOfStaminaPoints;
};
