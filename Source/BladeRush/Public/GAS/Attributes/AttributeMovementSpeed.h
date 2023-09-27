// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Attributes/AttributeBase.h"
#include "AttributeMovementSpeed.generated.h"

/**
 * 
 */
UCLASS()
class BLADERUSH_API UAttributeMovementSpeed : public UAttributeBase
{
	GENERATED_BODY()
public:
	static const UAttributeMovementSpeed* Find(const UAbilitySystemComponent* AbilitySystemComponent)
	{
		if (!AbilitySystemComponent) return nullptr;

		TSubclassOf<UAttributeMovementSpeed> MovementSpeed = UAttributeMovementSpeed::StaticClass();
		return Cast<UAttributeMovementSpeed>(AbilitySystemComponent->GetAttributeSet(MovementSpeed));
	};
	
	mutable FAttributeEvent OnOutOfStaminaPoints;
	
	ATTRIBUTE_ACCESSORS(UAttributeMovementSpeed, WalkSpeed);
	ATTRIBUTE_ACCESSORS(UAttributeMovementSpeed, RunSpeed);
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	//virtual void PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const override;

	UFUNCTION()
	void OnRep_WalkSpeed(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	void OnRep_RunSpeed(const FGameplayAttributeData& OldValue);
private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_WalkSpeed, Category = "Attributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData WalkSpeed;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_RunSpeed, Category = "Attributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData RunSpeed;
};
