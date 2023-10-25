// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Attributes/AttributeBase.h"
#include "MovementAttributeSet.generated.h"

/**
 * Movement attribute set. Contains all movement attributes
 */
UCLASS()
class BLADERUSH_API UMovementAttributeSet : public UAttributeBase
{
	GENERATED_BODY()
public:
	static const UMovementAttributeSet* Find(const UAbilitySystemComponent* AbilitySystemComponent)
	{
		if (!AbilitySystemComponent) return nullptr;

		TSubclassOf<UMovementAttributeSet> MovementAttributeSet = UMovementAttributeSet::StaticClass();
		return Cast<UMovementAttributeSet>(AbilitySystemComponent->GetAttributeSet(MovementAttributeSet));
	};
	
	ATTRIBUTE_ACCESSORS(UMovementAttributeSet, GrapplingHookLength);
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
protected:
	
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	//virtual void PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const override;

	UFUNCTION()
	void OnRep_GrapplingHookLength(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	void OnRep_GrapplingProjectileMaxDistance(const FGameplayAttributeData& OldValue);
	
private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_GrapplingHookLength, Category = "Attributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData GrapplingHookLength;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_GrapplingProjectileMaxDistance, Category = "Attributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData GrapplingProjectileMaxDistance;
};
