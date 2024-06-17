// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BladeRushGlobals.h"
#include "GAS/Attributes/AttributeBase.h"
#include "AttributeHitPoints.generated.h"

class ABaseCharacter;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FHitDamageReceivedDelegate, FHitResult, HitResult, FGameplayTagContainer,
                                               SourceTags, FGameplayEffectContextHandle, GameplayEffectSpec);
DECLARE_MULTICAST_DELEGATE_OneParam(FOutOfHitPointsDelegate, const FDeadPlayerInfo& DeadPlayerInfo);

UCLASS()
class BLADERUSH_API UAttributeHitPoints : public UAttributeBase
{
	GENERATED_BODY()
	
public:
	UAttributeHitPoints();

	static const UAttributeHitPoints* Find(const UAbilitySystemComponent* AbilitySystemComponent)
	{
		if (!AbilitySystemComponent) return nullptr;

		TSubclassOf<UAttributeHitPoints> HealthPoints = UAttributeHitPoints::StaticClass();
		return Cast<UAttributeHitPoints>(AbilitySystemComponent->GetAttributeSet(HealthPoints));
	};
	
	mutable FOutOfHitPointsDelegate OutOfHitPointsDelegate;
	mutable FHitDamageReceivedDelegate HitDamageReceivedDelegate;
	
	ATTRIBUTE_ACCESSORS(UAttributeHitPoints, HitPoints);
	ATTRIBUTE_ACCESSORS(UAttributeHitPoints, MaxHitPoints);
	ATTRIBUTE_ACCESSORS(UAttributeHitPoints, HitDamage);
	
	void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	/*Damage meta attributes*/
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay|Damage", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData HitDamage; // attribute used for hit damage. For example when bullet hits the player
	
protected:
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	virtual void ClampAttribute(const FGameplayAttribute& Attribute,float& NewValue) const;
	
	UFUNCTION()
	void OnRep_HitPoints(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_MaxHitPoints(const FGameplayAttributeData& OldValue);
	
private:
	UPROPERTY(EditAnywhere,BlueprintReadOnly, ReplicatedUsing = OnRep_HitPoints, Category = "Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData HitPoints;
	UPROPERTY(EditAnywhere,BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHitPoints, Category = "Healath", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHitPoints;
	
	bool bOutOfHitPoints;
	bool bPrevOutOfHitPoints;
	float OldValueCached;
};
